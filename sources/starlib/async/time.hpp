#pragma once
#include "starlib/types/starlib_stdint.hpp"

namespace starlib
{
    ///Highly precise sleep that minimizes CPU overhead, using a combination of system level accurate sleeping and spinlock.
    void sleep_nanos(const u64 nanoseconds);

    ///Highly precise sleep that minimizes CPU overhead, using a combination of system level accurate sleeping and spinlock.
    void sleep_micros(const u64 microseconds);

    ///Highly precise system timer.
    ///Note that system time may overflow and roll backwards in rare instances.
    u64 systime_nanos();

    ///Highly precise system timer.
    ///Note that system time may overflow and roll backwards in rare instances.
    u64 systime_micros();
}
