#include "time.hpp"
#include "nanotime/nanotime.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
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
