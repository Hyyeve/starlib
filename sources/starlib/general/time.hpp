#pragma once
#include "stdint.hpp"

namespace starlib
{
    ///Highly precise system timer.
    ///Note that system time may overflow and roll backwards in rare instances.
    u64 systime_nanos();

    ///Highly precise system timer.
    ///Note that system time may overflow and roll backwards in rare instances.
    u64 systime_micros();
}
