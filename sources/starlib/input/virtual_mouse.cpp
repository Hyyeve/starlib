#include "virtual_mouse.hpp"

#include <ranges>
#include <tracy/Tracy.hpp>

namespace starlib
{
    using namespace starlib;
    using namespace glm;

    bool virtual_mouse::is_clicked(const u64 id)
    {
        return button_map[id] > 0;
    }

    bool virtual_mouse::is_released(const u64 id)
    {
        return button_map[id] < 0;
    }

    bool virtual_mouse::was_clicked_this_frame(const u64 id)
    {
        return button_map[id] == 1;
    }

    bool virtual_mouse::was_released_this_frame(const u64 id)
    {
        return button_map[id] == -1;
    }

    i64 virtual_mouse::get_frame_count(const u64 id)
    {
        return button_map[id];
    }

    u64 virtual_mouse::get_pressed_frames(const u64 id)
    {
        return std::max(0ll, get_frame_count(id));
    }

    u64 virtual_mouse::get_released_frames(const u64 id)
    {
        return std::max(0ll, -get_frame_count(id));
    }

    dvec2 virtual_mouse::get_position_absolute() const
    {
        return mouse_position;
    }

    dvec2 virtual_mouse::get_position_delta() const
    {
        return mouse_delta;
    }

    dvec2 virtual_mouse::get_scroll_absolute() const
    {
        return absolute_scroll;
    }

    dvec2 virtual_mouse::get_scroll_delta() const
    {
        return delta_scroll;
    }

    u64 virtual_mouse::get_last_clicked() const
    {
        return last_clicked_id;
    }

    u64 virtual_mouse::get_last_released() const
    {
        return last_released_id;
    }

    void virtual_mouse::move_by(const dvec2 delta)
    {
        mouse_delta += delta;
        mouse_position += delta;
    }

    void virtual_mouse::move_to(const dvec2 position)
    {
        mouse_delta += position - mouse_position;
        mouse_position = position;
    }

    void virtual_mouse::scroll_by(const dvec2 delta)
    {
        delta_scroll += delta;
        absolute_scroll += delta;
    }

    void virtual_mouse::scroll_to(const dvec2 scroll)
    {
        delta_scroll += scroll - absolute_scroll;
        absolute_scroll = scroll;
    }

    void virtual_mouse::click(const u64 id)
    {
        button_map[id] = 1;
        last_clicked_id = id;
    }

    void virtual_mouse::release(const u64 id)
    {
        button_map[id] = -1;
        last_released_id = id;
    }

    void virtual_mouse::advance_frame()
    {
        ZoneScoped;
        mouse_delta = ivec2();
        delta_scroll = ivec2();
        for (i64& value : button_map | std::views::values)
        {
            if (value > 0) value++;
            else if (value < 0) value--;
        }
    }
}
