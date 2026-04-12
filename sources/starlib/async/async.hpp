#pragma once
#include "../general/stdint.hpp"

namespace starlib
{
    ///Get a numerical ID for the calling thread. Thread IDs are assigned on demand and increment with each known thread.
    u32 thread_id();

    ///Highly precise sleep that minimizes CPU overhead, using a combination of system level accurate sleeping and spinlock.
    void sleep_nanos(const u64 nanoseconds);

    ///Highly precise sleep that minimizes CPU overhead, using a combination of system level accurate sleeping and spinlock.
    void sleep_micros(const u64 microseconds);
}
