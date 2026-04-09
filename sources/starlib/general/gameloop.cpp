#include "gameloop.hpp"
#include "../math/glm.hpp"
#include "starlib/async/async.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
    void gameloop::run()
    {
        {
            ZoneScopedN("[Starlib] Gameloop launch");
            this->launch();
        }

        this->timer_clock.start();

        gameloop::loop_data loop_data {};

        f64 runtime = 0.0f;
        f64 current_time = loop_data.time;
        f64 update_accumulator = 0.0f;
        f64 frame_accumulator = 0.0f;
        loop_data.config = this;

        while (true)
        {
            ZoneScopedN("[Starlib] Gameloop iteration");
            const f64 max_step_time = this->max_update_time;
            const f64 update_step_time = 1.0 / this->target_ups;
            const f64 frame_step_time = 1.0 / this->target_fps;
            u32 update_budget = this->max_updates_per_render;

            const f64 new_time = this->timer_clock.get_elapsed();
            const f64 raw_delta_time = new_time - current_time;

            const f64 adjusted_delta_time = raw_delta_time > max_step_time ? max_step_time : raw_delta_time;

            update_accumulator += adjusted_delta_time;
            frame_accumulator += raw_delta_time;
            current_time = new_time;

            const f64 time_until_anything = glm::min(update_step_time - update_accumulator, frame_step_time - frame_accumulator);
            if (time_until_anything > 0)
            {
                sleep_micros(time_until_anything * 1000 * 1000);
                continue;
            }

            loop_data.performance_factor = update_accumulator / update_step_time;
            loop_data.delta_time = update_step_time;

            while (update_accumulator >= update_step_time && update_budget > 0)
            {
                ZoneScopedN("[Starlib] Gameloop update");
                this->update(loop_data);
                if (this->should_exit) break;

                update_budget--;
                runtime += update_step_time;
                update_accumulator -= update_step_time;
            }

            if (this->should_exit) break;

            if (frame_accumulator >= frame_step_time)
            {
                if (should_render)
                {
                    ZoneScopedN("[Starlib] Gameloop render");
                    const f64 interpolation_delta = glm::fract(update_accumulator) / update_step_time;
                    const f64 interpolated_runtime = runtime + update_step_time * interpolation_delta;
                    this->render({interpolated_runtime, frame_accumulator, interpolation_delta});
                }

                frame_accumulator = 0;
            }
        }

        ZoneScopedN("[Starlib] Gameloop exit");
        this->exit();
        this->timer_clock.stop();
    }
}
