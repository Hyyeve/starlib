#pragma once
#include "starlib/general/stdint.hpp"
#include "starlib/math/glm.hpp"

namespace starlib
{
    class virtual_mouse
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

        void move_by(glm::dvec2 delta);
        void move_to(glm::dvec2 position);
        void scroll_by(glm::dvec2 delta);
        void scroll_to(glm::dvec2 scroll);
        void click(u64 id);
        void release(u64 id);

        void advance_frame();
    protected:
        std::unordered_map<i64, i64> button_map = std::unordered_map<i64, i64>();
        glm::dvec2 mouse_position = glm::dvec2();
        glm::dvec2 mouse_delta = glm::dvec2();
        glm::dvec2 absolute_scroll = glm::dvec2();
        glm::dvec2 delta_scroll = glm::dvec2();
        u64 last_clicked_id = u64_max;
        u64 last_released_id = u64_max;
    };
}
