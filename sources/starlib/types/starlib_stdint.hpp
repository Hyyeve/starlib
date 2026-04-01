#pragma once
#include <atomic>

namespace starlib
{
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef int8_t i8;
    typedef int16_t i16;
    typedef int32_t i32;
    typedef int64_t i64;

    typedef float f32;
    typedef double f64;

    typedef std::atomic_bool atomic_bool;

    typedef std::atomic_uint8_t atomic_u8;
    typedef std::atomic_uint16_t atomic_u16;
    typedef std::atomic_uint32_t atomic_u32;
    typedef std::atomic_uint64_t atomic_u64;

    typedef std::atomic_int8_t atomic_i8;
    typedef std::atomic_int16_t atomic_i16;
    typedef std::atomic_int32_t atomic_i32;
    typedef std::atomic_int64_t atomic_i64;

    constexpr u8 u8_max = UINT8_MAX;
    constexpr u16 u16_max = UINT16_MAX;
    constexpr u32 u32_max = UINT32_MAX;
    constexpr u64 u64_max = UINT64_MAX;

    constexpr i8 i8_min = INT8_MIN;
    constexpr i16 i16_min = INT16_MIN;
    constexpr i32 i32_min = INT32_MIN;
    constexpr i64 i64_min = INT64_MIN;

    constexpr i8 i8_max = INT8_MAX;
    constexpr i16 i16_max = INT16_MAX;
    constexpr i32 i32_max = INT32_MAX;
    constexpr i64 i64_max = INT64_MAX;
}
