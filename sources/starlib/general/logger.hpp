#pragma once
#include <mutex>
#include <unordered_map>

#include "../utility/string.hpp"
#include "stdint.hpp"

namespace starlib
{
    ///Fairly simple threadsafe logger with formatting
    class logger
    {
    public:
        enum class log_level : u8
        {
            ALL = 3, //Everything!
            NORMAL = 2, //No debug
            REDUCED = 1, //Only warn / error / fatal
            FATAL_ONLY = 0 //Only fatal
        };

        struct logger_tag
        {
            logger_tag(const std::string_view& tag, const std::string_view& format_codes) noexcept : tag_id(tag), message_format_codes(format_codes) {}

            std::string tag_id;
            std::string message_format_codes;
        };

        ///Destructing the logger automatically flushes any remaining logs.
        ~logger();

        void set_log_level(const log_level level);

        ///Flush buffer logs. A good time to call this is at the end of your update/frame loop.
        ///Safe to call on a separate thread or through a threadpool task.
        ///Note that for performance reasons, timestamps are generated at the time of the flush, *not* at the time of the log call.
        void flush_logs();

        ///Root log function
        void log_str(const logger_tag& tag, const std::string_view msg, const std::string_view type, const std::string_view formatting);

        //Predefined varients for common use cases:

        void log_debug_str(const logger_tag& tag, const std::string_view msg);
        void log_info_str(const logger_tag& tag, const std::string_view msg);
        void log_performance_str(const logger_tag& tag, const std::string_view msg);
        void log_warn_str(const logger_tag& tag, const std::string_view msg);
        void log_error_str(const logger_tag& tag, const std::string_view msg);
        void log_fatal_str(const logger_tag& tag, const std::string_view msg);

        //Templated versions of the predefined functions that performs automatic std::format formatting

        inline void log_debug(const logger_tag tag, const auto... params) { log_debug_str(tag, stringify(params...)); }
        inline void log_info(const logger_tag tag, const auto... params) { log_info_str(tag, stringify(params...)); }
        inline void log_performance(const logger_tag tag, const auto... params) { log_performance_str(tag, stringify(params...)); }
        inline void log_warn(const logger_tag tag, const auto... params) { log_warn_str(tag, stringify(params...)); }
        inline void log_error(const logger_tag tag, const auto... params) { log_error_str(tag, stringify(params...)); }
        inline void log_fatal(const logger_tag tag, const auto... params) { log_fatal_str(tag, stringify(params...)); }

    private:
        void write_tag(std::string_view format, std::string_view color_code, const logger_tag& tag);

        std::mutex logging_lock;
    #ifdef DEBUG
        log_level logging_level = log_level::ALL;
    #else
        log_level logging_level = log_level::NORMAL;
    #endif

        std::stringstream builder;
        std::vector<std::string> message_queue;
        std::unordered_map<u64, u64> message_repeat_map;
        u64 last_message_hash = 0;
    };
}
