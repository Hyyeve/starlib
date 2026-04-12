#pragma once
#include <string>
#include <string_view>
#include "stdint.hpp"

namespace starlib
{
    enum class status_type : u8
    {
        SUCCESS, UNSUPPORTED, UNIMPLEMENTED, NOTHING_TO_DO,
        ALREADY_INITIALIZED, NOT_INITIALIZED,
        UNKNOWN, DUPLICATE, UNEXPECTED, RANGE_OVERFLOW, TIMEOUT,
        BACKEND_ERROR, INVALID,
    };

    struct status
    {
        // ReSharper disable once CppNonExplicitConvertingConstructor
        status(const status_type type) : type{type}, message("No message provided") {}
        status(const status_type type, const std::string_view& message) : type{type}, message(message) {}

        status_type type;
        std::string message;

        [[nodiscard]] inline bool is_error() const
        {
            switch (type)
            {
                case status_type::NOTHING_TO_DO:
                case status_type::SUCCESS: return false;
                default: return true;
            }
        }
    };
}
