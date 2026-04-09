#include "../general/logger.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <tracy/Tracy.hpp>

#include "../utility/string.hpp"
#include "starlib/async/async.hpp"

namespace starlib
{
    logger::~logger()
    {
        flush_logs();
    }

    void logger::set_log_level(const log_level level)
    {
        logging_lock.lock();
        logging_level = level;
        logging_lock.unlock();
    }

    void logger::flush_logs()
    {
        ZoneScoped;
        logging_lock.lock();

        if (message_queue.empty())
        {
            logging_lock.unlock();
            return;
        }

        //Minimize blocking other threads logging messages
        const std::vector messages = std::move(message_queue);
        message_queue = std::vector<std::string>();
        logging_lock.unlock();

        const auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now())};
        const std::string time_str = std::format("{0:%T}", local_time);
        const std::string timestamp = stringify('[', ansi_formatting::bold, ansi_formatting::cyan, time_str, ansi_formatting::reset, "]");

        u64 last_message_hash = 0;

        for (const std::string& message : messages)
        {
            const u64 hash = string_hash(message);
            const bool is_duplicate = hash == last_message_hash;
            if (is_duplicate) std::cout << ansi_formatting::delete_line;

            std::cout << timestamp;
            std::cout << message;

            if (is_duplicate) std::cout << " (" << ansi_formatting::bold << "x" << std::to_string(message_repeat_map[hash]) << ')';

            last_message_hash = hash;
            message_repeat_map[hash]++;
        }
    }

    void logger::log_str(const logger_tag& tag, const std::string_view msg, const std::string_view type, const std::string_view formatting)
    {
        ZoneScoped;

        logging_lock.lock();

        builder << stringify("[", ansi_formatting::bold, ansi_formatting::bright_blue, "T", std::setfill('0'), std::setw(3), thread_id(), ansi_formatting::reset, ']');

        if (!type.empty()) write_tag(ansi_formatting::bold, formatting, {type, ""});
        if (!tag.tag_id.empty()) write_tag(ansi_formatting::reset, ansi_formatting::purple, tag);

        builder << ' ' << formatting << msg;

        message_queue.push_back(builder.str());
        builder.str("");

        logging_lock.unlock();
    }

    void logger::log_debug_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::NORMAL) log_str(tag, msg, "DEBG", ansi_formatting::lime);
    }

    void logger::log_info_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::REDUCED) log_str(tag, msg, "INFO", ansi_formatting::azure);
    }

    void logger::log_performance_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::REDUCED) log_str(tag, msg, "PERF", ansi_formatting::cyan);
    }

    void logger::log_warn_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::FATAL_ONLY) log_str(tag, msg, "WARN", ansi_formatting::bright_yellow);
    }

    void logger::log_error_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::FATAL_ONLY) log_str(tag, msg, "ERRR", ansi_formatting::red);
    }

    void logger::log_fatal_str(const logger_tag& tag, const std::string_view msg)
    {
        static const std::string fatal_format = stringify(ansi_formatting::bright_red, ansi_formatting::bold, ansi_formatting::underline);
        log_str(tag, msg, "FATAL", fatal_format);
    }

    inline void logger::write_tag(const std::string_view format, const std::string_view color_code, const logger_tag& tag)
    {
        builder << '[' << format << color_code << tag.tag_id << ansi_formatting::reset << ']' << tag.message_format_codes;
    }

    inline void logger::write_repeat_tag(const std::string_view color_code)
    {
    }
}
