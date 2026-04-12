#include "input.hpp"

#include <ranges>
#include <tracy/Tracy.hpp>

#include "starlib/utility/string.hpp"

namespace starlib
{
    bool keyboard_input::is_pressed(const u64 id)
    {
        return key_map[id] > 0;
    }

    bool keyboard_input::is_released(const u64 id)
    {
        return key_map[id] < 0;
    }

    bool keyboard_input::was_pressed_this_frame(const u64 id)
    {
        return get_frame_count(id) == 1;
    }

    bool keyboard_input::was_released_this_frame(const u64 id)
    {
        return get_frame_count(id) == -1;
    }

    i64 keyboard_input::get_frame_count(const u64 id)
    {
        return key_map[id];
    }

    u64 keyboard_input::get_pressed_frames(const u64 id)
    {
        return std::max(0ll, get_frame_count(id));
    }

    u64 keyboard_input::get_released_frames(const u64 id)
    {
        return std::max(0ll, -get_frame_count(id));
    }

    std::string keyboard_input::get_typed_text() const
    {
        return current_text;
    }

    u64 keyboard_input::get_last_pressed() const
    {
        return last_pressed_id;
    }

    u64 keyboard_input::get_last_released() const
    {
        return last_released_id;
    }

    u64 keyboard_input::get_last_typed_codepoint() const
    {
        return last_typed_codepoint;
    }

    void virtual_keyboard_input::type_character(const u64 codepoint)
    {
        current_text += stringify_utf32(codepoint);
        last_typed_codepoint = codepoint;
    }

    void virtual_keyboard_input::press_key(const u64 id)
    {
        key_map[id] = 1;
        last_pressed_id = id;
    }

    void virtual_keyboard_input::release_key(const u64 id)
    {
        key_map[id] = -1;
        last_released_id = id;
    }

    void virtual_keyboard_input::advance_frame()
    {
        ZoneScoped;
        for (i64& value : key_map | std::views::values)
        {
            if (value > 0) value++;
            else if (value < 0) value--;
        }

        current_text.clear();
    }
}
