#pragma once
#include "../general/stdint.hpp"

namespace starlib
{
    class clock
    {
    public:
        ///Start the clock. Elapsed time will start being counted.
        void start();

        ///Stop the clock. Elapsed time will stop being counted until start() is called.
        void stop();

        //Set the current clock time, in seconds.
        void set_elapsed(const f64 seconds);

        //Set the current clock time, in milliseconds.
        void set_elapsed_ms(const f64 milliseconds);

        //Set the current clock time, in microseconds.
        void set_elapsed_us(const u64 microseconds);

        ///Get the elapsed time, in seconds. Precision of the time is, at best, in microseconds.
        f64 get_elapsed();
        ///Get the elapsed time, in milliseconds. Precision of the time is, at best, in microseconds.
        f64 get_elapsed_ms();
        ///Get the elapsed time, in microseconds. Precision of the time is, at best, in microseconds.
        u64 get_elapsed_us();

    private:
        u64 last_seen_time = 0;
        u64 stored_microseconds = 0;
        bool running = false;
    };
}