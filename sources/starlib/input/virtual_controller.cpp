#include "virtual_controller.hpp"
#include <ranges>
#include "tracy/Tracy.hpp"

namespace starlib
{
    using namespace starlib;
    bool virtual_controller::is_pressed(const u64 id)
    {
        return button_map[id] > 0;
    }

    bool virtual_controller::is_released(const u64 id)
    {
        return button_map[id] < 0;
    }

    bool virtual_controller::was_pressed_this_frame(const u64 id)
    {
        return get_frame_count(id) == 1;
    }

    bool virtual_controller::was_released_this_frame(const u64 id)
    {
        return get_frame_count(id) == -1;
    }

    f32 virtual_controller::axis_value(const u64 id)
    {
        return axis_map[id];
    }

    bool virtual_controller::is_axis_pressed(const u64 id) {
        return axis_pressed_map[id] > 0;
    }

    bool virtual_controller::is_axis_released(const u64 id)
    {
        return axis_pressed_map[id] < 0;
    }

    bool virtual_controller::was_axis_pressed_this_frame(const u64 id)
    {
        return axis_pressed_map[id] == 1;
    }

    bool virtual_controller::was_axis_released_this_frame(const u64 id)
    {
        return axis_pressed_map[id] == -1;
    }

    i64 virtual_controller::get_frame_count(const u64 id)
    {
        return button_map[id];
    }

    u64 virtual_controller::get_pressed_frames(const u64 id)
    {
        return std::max(0ll, get_frame_count(id));
    }

    u64 virtual_controller::get_released_frames(const u64 id)
    {
        return std::max(0ll, -get_frame_count(id));
    }

    i64 virtual_controller::get_axis_frame_count(const u64 id)
    {
        return axis_pressed_map[id];
    }

    u64 virtual_controller::get_axis_pressed_frames(const u64 id)
    {
        return std::max(0ll, get_axis_frame_count(id));
    }

    u64 virtual_controller::get_axis_released_frames(const u64 id)
    {
        return std::max(0ll, -get_axis_frame_count(id));
    }

    u64 virtual_controller::get_last_pressed_button() const
    {
        return last_pressed_button;
    }

    u64 virtual_controller::get_last_released_button() const
    {
        return last_released_button;
    }

    u64 virtual_controller::get_last_pressed_axis() const
    {
        return last_pressed_axis;
    }

    u64 virtual_controller::get_last_released_axis() const
    {
        return last_released_axis;
    }

    void virtual_controller::press_button(const u64 id)
    {
        button_map[id] = 1;
        last_pressed_button = id;
    }

    void virtual_controller::release_button(const u64 id)
    {
        button_map[id] = -1;
        last_released_button = id;
    }

    void virtual_controller::set_axis(const u64 id, const f32 value)
    {
        axis_map[id] = value;
        if (value > axis_pressed_threshold && !is_axis_pressed(id))
        {
            axis_pressed_map[id] = 1;
            last_pressed_axis = id;
        }
        else if (value < axis_pressed_threshold && is_axis_pressed(id))
        {
            axis_pressed_map[id] = -1;
            last_released_axis = id;
        }
    }

    void virtual_controller::set_axis_press_threshold(const f32 threshold)
    {
        axis_pressed_threshold = threshold;
    }

    void virtual_controller::advance_frame()
    {
        ZoneScoped;
        for (i64& value : button_map | std::views::values)
        {
            if (value > 0) value++;
            else if (value < 0) value--;
        }

        for (i64& value : axis_pressed_map | std::views::values)
        {
            if (value > 0) value++;
            else if (value < 0) value--;
        }
    }
}
