#pragma once
#include <unordered_map>

#include "starlib/general/stdint.hpp"

namespace starlib
{
    class virtual_controller
    {
    public:
        [[nodiscard]] bool is_pressed(u64 id);
        [[nodiscard]] bool is_released(u64 id);
        [[nodiscard]] bool was_pressed_this_frame(u64 id);
        [[nodiscard]] bool was_released_this_frame(u64 id);

        [[nodiscard]] f32 axis_value(u64 id);
        [[nodiscard]] bool is_axis_pressed(u64 id);
        [[nodiscard]] bool is_axis_released(u64 id);
        [[nodiscard]] bool was_axis_pressed_this_frame(u64 id);
        [[nodiscard]] bool was_axis_released_this_frame(u64 id);

        [[nodiscard]] i64 get_frame_count(u64 id);
        [[nodiscard]] u64 get_pressed_frames(u64 id);
        [[nodiscard]] u64 get_released_frames(u64 id);

        [[nodiscard]] i64 get_axis_frame_count(u64 id);
        [[nodiscard]] u64 get_axis_pressed_frames(u64 id);
        [[nodiscard]] u64 get_axis_released_frames(u64 id);

        [[nodiscard]] u64 get_last_pressed_button() const;
        [[nodiscard]] u64 get_last_released_button() const;
        [[nodiscard]] u64 get_last_pressed_axis() const;
        [[nodiscard]] u64 get_last_released_axis() const;

        void press_button(u64 id);
        void release_button(u64 id);
        void set_axis(u64 id, f32 value);
        void set_axis_press_threshold(f32 threshold);

        void advance_frame();
    protected:
        std::unordered_map<u64, i64> button_map = std::unordered_map<u64, i64>();
        std::unordered_map<u64, f32> axis_map = std::unordered_map<u64, f32>();
        std::unordered_map<u64, i64> axis_pressed_map = std::unordered_map<u64, i64>();
        f32 axis_pressed_threshold = 0.1f;

        u64 last_pressed_button = u64_max;
        u64 last_released_button = u64_max;
        u64 last_pressed_axis = u64_max;
        u64 last_released_axis = u64_max;
    };
}
