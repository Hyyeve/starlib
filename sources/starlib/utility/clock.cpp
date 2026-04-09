#include "clock.hpp"

#include "time.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
    void clock::start()
    {
        if (running) return;
        last_seen_time = systime_micros();
        running = true;
    }

    void clock::stop()
    {
        running = false;
    }

    void clock::set_elapsed(const f64 seconds)
    {
        set_elapsed_ms(seconds * 1000.0f);
    }

    void clock::set_elapsed_ms(const f64 milliseconds)
    {
        set_elapsed_us(milliseconds * 1000.0f);
    }

    void clock::set_elapsed_us(const u64 microseconds)
    {
        stored_microseconds = microseconds;
    }

    f64 clock::get_elapsed()
    {
        return get_elapsed_ms() / 1000.0f;
    }

    f64 clock::get_elapsed_ms()
    {
        return get_elapsed_us() / 1000.0f;
    }

    u64 clock::get_elapsed_us()
    {
        ZoneScoped;
        if (!running) return stored_microseconds;
        const u64 now = systime_micros();

        if (last_seen_time > now) last_seen_time = now; //Uh oh. System time rolled over. Best we can do is pretend no time elapsed.

        const u64 elapsed = now - last_seen_time;
        stored_microseconds += elapsed;
        last_seen_time = now;

        return stored_microseconds;
    }
}
