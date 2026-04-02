#include "time.hpp"
#include "nanotime/nanotime.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
    void sleep_nanos(const u64 nanoseconds)
    {
        ZoneScoped;
        nanotime::nanotime_sleep(nanoseconds);
    }

    void sleep_micros(const u64 microseconds)
    {
        sleep_nanos(microseconds * 1000);
    }

    u64 systime_nanos()
    {
        ZoneScoped;
        return nanotime::nanotime_now();
    }

    u64 systime_micros()
    {
        return systime_nanos() / 1000;
    }
}
