#pragma once
#include <map>
#include <string>

#include "starlib/general/stdint.hpp"
#include "starlib/math/glm.hpp"

namespace starlib
{
    class keyboard_input
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

    protected:
        std::map<i64, i64> key_map = std::map<i64, i64>();
        std::string current_text;
        u64 last_pressed_id = u64_max;
        u64 last_released_id = u64_max;
        u64 last_typed_codepoint = u64_max;
    };

    class virtual_keyboard_input : public keyboard_input
    {
    public:
        void type_character(u64 codepoint);
        void press_key(u64 id);
        void release_key(u64 id);

        void advance_frame();
    };

    class mouse_input
    {
    public:
        [[nodiscard]] bool is_clicked(u64 id);
        [[nodiscard]] bool is_released(u64 id);
        [[nodiscard]] bool was_clicked_this_frame(u64 id);
        [[nodiscard]] bool was_released_this_frame(u64 id);

        ///Positive values indicate that the button is being held down, negative values indicate that it is released
        ///A zero value indicates an unknown state.
        [[nodiscard]] i64 get_frame_count(u64 id);
        [[nodiscard]] u64 get_pressed_frames(u64 id);
        [[nodiscard]] u64 get_released_frames(u64 id);

        [[nodiscard]] glm::dvec2 get_position_absolute() const;
        [[nodiscard]] glm::dvec2 get_position_delta() const;
        [[nodiscard]] glm::dvec2 get_scroll_absolute() const;
        [[nodiscard]] glm::dvec2 get_scroll_delta() const;

        [[nodiscard]] u64 get_last_clicked() const;
        [[nodiscard]] u64 get_last_released() const;

    protected:
        std::map<i64, i64> button_map = std::map<i64, i64>();
        glm::dvec2 mouse_position = glm::dvec2();
        glm::dvec2 mouse_delta = glm::dvec2();
        glm::dvec2 absolute_scroll = glm::dvec2();
        glm::dvec2 delta_scroll = glm::dvec2();
        u64 last_clicked_id = u64_max;
        u64 last_released_id = u64_max;
    };

    class virtual_mouse_input : public mouse_input
    {
    public:
        void move_by(glm::dvec2 delta);
        void move_to(glm::dvec2 position);
        void scroll_by(glm::dvec2 delta);
        void scroll_to(glm::dvec2 scroll);
        void click(u64 id);
        void release(u64 id);

        void advance_frame();
    };

    class controller_input
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

        void set_axis_press_threshold(f32 threshold);

    protected:
        std::map<u64, i64> button_map = std::map<u64, i64>();
        std::map<u64, f32> axis_map = std::map<u64, f32>();
        std::map<u64, i64> axis_pressed_map = std::map<u64, i64>();
        f32 axis_pressed_threshold = 0.1f;

        u64 last_pressed_button = u64_max;
        u64 last_released_button = u64_max;
        u64 last_pressed_axis = u64_max;
        u64 last_released_axis = u64_max;
    };

    class virtual_controller_input : public controller_input
    {
    public:
        void press_button(u64 id);
        void release_button(u64 id);

        void set_axis(u64 id, f32 value);

        void advance_frame();
    };
}
