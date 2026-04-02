#include "logger.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <tracy/Tracy.hpp>

#include "string.hpp"
#include "starlib/async/async.hpp"

namespace starlib::logger
{
    static std::mutex logging_lock;
    #ifdef DEBUG
    static log_level logging_level = log_level::ALL;
    #else
    static log_level logging_level = log_level::NORMAL;
    #endif

    static i32 log_repeat_count = 0;
    static std::string last_message;
    static logger_tag last_tag = {"None", ""};
    static std::stringstream builder;
    static std::vector<std::string> message_queue;


    void set_log_level(const log_level level)
    {
        logging_lock.lock();
        logging_level = level;
        logging_lock.unlock();
    }

    void flush_logs()
    {
        ZoneScoped;
        logging_lock.lock();

        if (message_queue.empty())
        {
            logging_lock.unlock();
            return;
        }

        const std::vector messages = message_queue;
        message_queue.clear();
        logging_lock.unlock();

        const auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()};
        const std::string time_str = std::format("%T", local_time);
        const std::string timestamp = stringify('[', ansi_formatting::bold, ansi_formatting::cyan, time_str, ansi_formatting::reset, "]");

        for (const std::string& message : messages)
        {
            if (message != ansi_formatting::delete_line) std::cout << timestamp;
            std::cout << message;
        }
    }

    inline void write_tag(const std::string_view format, const std::string_view color_code, const logger_tag& tag)
    {
        builder << '[' << format << color_code << tag.tag_id << ansi_formatting::reset << ']' << tag.message_format_codes;
    }

    inline void write_repeat_tag(const std::string_view color_code)
    {
        builder << ansi_formatting::reset << " [" << ansi_formatting::bold << color_code << "+" << std::to_string(log_repeat_count) << ansi_formatting::reset << ']';
    }

    void log_str(const logger_tag& tag, const std::string_view msg, const std::string_view type, const std::string_view formatting)
    {
        ZoneScoped;

        logging_lock.lock();

        const bool is_repeat = msg == last_message && tag.tag_id == last_tag.tag_id;

        if (is_repeat)
        {
            log_repeat_count++;
            message_queue.emplace_back(ansi_formatting::delete_line);
        }
        else
        {
            log_repeat_count = 0;
            last_message = msg;
            last_tag = tag;
        }

        builder << stringify("[", ansi_formatting::bold, ansi_formatting::bright_blue, "T", std::setfill('0'), std::setw(3), async::thread_id(), ansi_formatting::reset, ']');

        if (!type.empty()) write_tag(ansi_formatting::bold, formatting, {type, ""});
        if (!tag.tag_id.empty()) write_tag(ansi_formatting::reset, ansi_formatting::purple, tag);

        builder << ' ' << formatting << msg << ansi_formatting::reset;

        if (is_repeat) write_repeat_tag(formatting);

        builder << '\n';

        message_queue.push_back(builder.str());
        builder.str("");

        logging_lock.unlock();
    }

    void log_debug_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::NORMAL) log_str(tag, msg, "DEBG", ansi_formatting::lime);
    }

    void log_info_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::REDUCED) log_str(tag, msg, "INFO", ansi_formatting::azure);
    }

    void log_performance_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::REDUCED) log_str(tag, msg, "PERF", ansi_formatting::cyan);
    }

    void log_warn_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::FATAL_ONLY) log_str(tag, msg, "WARN", ansi_formatting::bright_yellow);
    }

    void log_error_str(const logger_tag& tag, const std::string_view msg)
    {
        if (logging_level > log_level::FATAL_ONLY) log_str(tag, msg, "ERRR", ansi_formatting::red);
    }

    void log_fatal_str(const logger_tag& tag, const std::string_view msg)
    {
        static const std::string fatal_format = stringify(ansi_formatting::bright_red, ansi_formatting::bold, ansi_formatting::underline);
        log_str(tag, msg, "FATAL", fatal_format);
    }
}
