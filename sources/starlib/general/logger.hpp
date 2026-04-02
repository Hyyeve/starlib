#pragma once
#include "string.hpp"
#include "starlib/types/starlib_stdint.hpp"

namespace starlib
{
    ///Fairly simple threadsafe logger with formatting
    inline namespace logger
    {
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

        void set_log_level(const log_level level);

        ///Flush buffer logs. A good time to call this is at the end of your update/frame loop.
        ///Safe to call on a separate thread or through a threadpool task.
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

        #define VARARG_DEF(type) void type(const logger_tag tag, const std::string_view msg, const auto... params) { type_str(tag, std::format(msg, params...)); }

        VARARG_DEF(log_debug)
        VARARG_DEF(log_info)
        VARARG_DEF(log_performance)
        VARARG_DEF(log_warn)
        VARARG_DEF(log_error)
        VARARG_DEF(log_fatal)

        #undef VARARG_DEF
    }
}
