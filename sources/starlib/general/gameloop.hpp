#pragma once
#include <functional>
#include "stdint.hpp"
#include "clock.hpp"

namespace starlib
{
    struct gameloop
    {
        ///Information passed to each update/render call. Not all information is available to both calls.
        struct loop_data
        {
            ///Time since the gameloop started
            f64 time;

            ///Time since the last frame or update, depending on context.
            f64 delta_time;

            ///Interpolation value that represents how far between the previous update and the current update this frame is. Should be used alongside interpolable<x> for smooth rendering interpolation.
            f64 interpolation;

            ///Only valid in the update call:
            ///Indicates how fast the gameloop is running compared to realtime.
            ///It's the ratio of actual updates / expected updates.
            ///1.0 is exactly realtime, <1.0 is some percentage of realtime, and >1.0 is faster than realtime (caused by falling behind and trying to catch up)
            ///Some fluctuation is normal.
            f64 performance_factor;

            ///Only valid in the update call:
            ///Game loop reference. Can be modified by the update call to adjust settings or end the gameloop.
            gameloop* config;
        };

        ///Target updates per second
        u32 target_ups = 100;

        ///Target frames per second.
        u32 target_fps = 240;

        ///Max time an update can take, in seconds. If updates take longer than this, the gameloop will appear to slow down (timings will no longer match realtime)
        f64 max_update_time = 0.05;

        ///Max updates that can happen per render call.
        u32 max_updates_per_render = 5;

        ///Whether the rendering callback should be called
        bool should_render = true;

        ///Whether the frame loop should exit. This flag must be set for the frame loop to finish running.
        bool should_exit = false;

        ///Called once when the gameloop begins.
        std::function<void()> launch = [] {};

        ///Called every update
        std::function<void(const loop_data&)> update = [](const loop_data& data) { data.config->should_exit = true; };

        ///Called every render
        std::function<void(const loop_data&)> render = [](const loop_data& data) {};

        ///Called once when the gameloop is exiting.
        std::function<void()> exit = [] {};

        ///Clock used to track the loop timers.
        clock timer_clock;

        ///Starts executing the gameloop. Blocks until the gameloop exits!
        void run();
    };
}
