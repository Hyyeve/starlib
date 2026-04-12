#pragma once
#include <string>
#include <unordered_map>

#include "starlib/general/stdint.hpp"

namespace starlib
{
    class virtual_keyboard
    {
    public:
        [[nodiscard]] bool is_pressed(u64 id);
        [[nodiscard]] bool is_released(u64 id);
        [[nodiscard]] bool was_pressed_this_frame(u64 id);
        [[nodiscard]] bool was_released_this_frame(u64 id);

        ///Positive values indicate that the button is being held down, negative values indicate that it is released
        ///A zero value indicates an unknown state.
        [[nodiscard]] i64 get_frame_count(u64 id);
        [[nodiscard]] u64 get_pressed_frames(u64 id);
        [[nodiscard]] u64 get_released_frames(u64 id);

        [[nodiscard]] std::string get_typed_text() const;

        [[nodiscard]] u64 get_last_pressed() const;
        [[nodiscard]] u64 get_last_released() const;
        [[nodiscard]] u64 get_last_typed_codepoint() const;

        void type_character(u64 codepoint);
        void press_key(u64 id);
        void release_key(u64 id);

        void advance_frame();

    protected:
        std::unordered_map<i64, i64> key_map = std::unordered_map<i64, i64>();
        std::string current_text;
        u64 last_pressed_id = u64_max;
        u64 last_released_id = u64_max;
        u64 last_typed_codepoint = u64_max;
    };
}
