#pragma once
#include "string.hpp"
#include "glm/glm.hpp"

namespace starlib_colors
{
    using namespace starlib;
    using namespace starlib_stdint;

    constexpr glm::vec4 white = glm::vec4(1, 1, 1, 1);
    constexpr glm::vec4 black = glm::vec4(0, 0, 0, 1);
    constexpr glm::vec4 transparent_white = glm::vec4(1, 1, 1, 0);
    constexpr glm::vec4 transparent_black = glm::vec4(0, 0, 0, 0);

    constexpr glm::vec4 red = glm::vec4(1, 0, 0, 1);
    constexpr glm::vec4 green = glm::vec4(0, 1, 0, 1);
    constexpr glm::vec4 blue = glm::vec4(0, 0, 1, 1);
    constexpr glm::vec4 yellow = glm::vec4(1, 1, 0, 1);
    constexpr glm::vec4 cyan = glm::vec4(0, 1, 1, 1);
    constexpr glm::vec4 magenta = glm::vec4(1, 0, 1, 1);

    constexpr glm::vec4 light_gray = glm::vec4(0.75, 0.75, 0.75, 1);
    constexpr glm::vec4 light_red = glm::vec4(1, 0.5, 0.5, 1);
    constexpr glm::vec4 light_green = glm::vec4(1, 0.5, 0.5, 1);
    constexpr glm::vec4 light_blue = glm::vec4(0.5, 0.5, 1, 1);
    constexpr glm::vec4 light_yellow = glm::vec4(1, 1, 0.5, 1);
    constexpr glm::vec4 light_cyan = glm::vec4(0.5, 1, 1, 1);
    constexpr glm::vec4 light_magenta = glm::vec4(1, 0.5, 1, 1);
    constexpr glm::vec4 light_orange = glm::vec4(1, 0.75, 0.25, 1);
    constexpr glm::vec4 light_mint = glm::vec4(0.25, 1, 0.75, 1);
    constexpr glm::vec4 light_violet = glm::vec4(0.75, 0.25, 1, 1);
    constexpr glm::vec4 light_crimson = glm::vec4(1, 0.25, 0.75, 1);
    constexpr glm::vec4 light_lime = glm::vec4(0.75, 1, 0.25, 1);
    constexpr glm::vec4 light_azure = glm::vec4(0.25, 0.75, 1, 1);

    constexpr glm::vec4 gray = glm::vec4(0.5, 0.5, 0.5, 1);
    constexpr glm::vec4 orange = glm::vec4(1, 0.5, 0, 1);
    constexpr glm::vec4 mint = glm::vec4(0, 1, 0.5, 1);
    constexpr glm::vec4 violet = glm::vec4(0.5, 0, 1, 1);
    constexpr glm::vec4 crimson = glm::vec4(1, 0, 0.5, 1);
    constexpr glm::vec4 lime = glm::vec4(0.5, 1, 0, 1);
    constexpr glm::vec4 azure = glm::vec4(0, 0.5, 1, 1);

    constexpr glm::vec4 half_red = glm::vec4(0.5, 0, 0, 1);
    constexpr glm::vec4 half_green = glm::vec4(0, 0.5, 0, 1);
    constexpr glm::vec4 half_blue = glm::vec4(0, 0, 0.5, 1);
    constexpr glm::vec4 half_yellow = glm::vec4(0.5, 0.5, 0, 1);
    constexpr glm::vec4 half_cyan = glm::vec4(0, 0.5, 0.5, 1);
    constexpr glm::vec4 half_magenta = glm::vec4(0.5, 0, 0.5, 1);

    constexpr glm::vec4 dark_gray = glm::vec4(0.25, 0.25, 0.25, 1);
    constexpr glm::vec4 dark_red = glm::vec4(0.25, 0, 0, 1);
    constexpr glm::vec4 dark_green = glm::vec4(0, 0.25, 0, 1);
    constexpr glm::vec4 dark_blue = glm::vec4(0, 0, 0.25, 1);
    constexpr glm::vec4 dark_yellow = glm::vec4(0.25, 0.25, 0, 1);
    constexpr glm::vec4 dark_cyan = glm::vec4(0, 0.25, 0.25, 1);
    constexpr glm::vec4 dark_magenta = glm::vec4(0.25, 0, 0.25, 1);
    constexpr glm::vec4 dark_orange = glm::vec4(0.5, 0.25, 0, 1);
    constexpr glm::vec4 dark_mint = glm::vec4(0, 0.5, 0.25, 1);
    constexpr glm::vec4 dark_violet = glm::vec4(0.25, 0, 0.5, 1);
    constexpr glm::vec4 dark_crimson = glm::vec4(0.5, 0, 0.25, 1);
    constexpr glm::vec4 dark_lime = glm::vec4(0.25, 0.5, 0, 1);
    constexpr glm::vec4 dark_azure = glm::vec4(0, 0.25, 0.5, 1);

    ///Converts hex 0xRRGGBBAA values to 4-channel RGBA float
    [[nodiscard]] constexpr glm::vec4 bits_to_color(const u32 hex)
    {
        return {
            static_cast<f32>(hex >> 24u & 0xFFu) / 255.0f,
            static_cast<f32>(hex >> 16u & 0xFFu) / 255.0f,
            static_cast<f32>(hex >> 8u & 0xFFu) / 255.0f,
            static_cast<f32>(hex >> 0u & 0xFFu) / 255.0f,
        };
    }

    ///Converts 4-channel RGBA float to a hex 0xRRGGBBAA value
    [[nodiscard]] constexpr u32 color_to_bits(const glm::vec4 col)
    {
        const u32 byte_r = static_cast<u8>(col.r * 255);
        const u32 byte_g = static_cast<u8>(col.g * 255);
        const u32 byte_b = static_cast<u8>(col.b * 255);
        const u32 byte_a = static_cast<u8>(col.a * 255);
        return byte_r << 24u | (byte_g << 16u) | (byte_b << 8u) | byte_a;
    }

    ///Returns the value of a named color constant by name
    [[nodiscard]] constexpr glm::vec4 color_from_name(const std::string_view name)
    {
        switch (string_hash(name))
        {
            case string_hash("white"): return white;
            case string_hash("black"): return black;
            case string_hash("transparent_white"): return transparent_white;
            case string_hash("transparent_black"): return transparent_black;

            case string_hash("red"): return red;
            case string_hash("green"): return green;
            case string_hash("blue"): return blue;
            case string_hash("yellow"): return yellow;
            case string_hash("cyan"): return cyan;
            case string_hash("magenta"): return magenta;

            case string_hash("light_gray"): return light_gray;
            case string_hash("light_red"): return light_red;
            case string_hash("light_green"): return light_green;
            case string_hash("light_blue"): return light_blue;
            case string_hash("light_yellow"): return light_yellow;
            case string_hash("light_cyan"): return light_cyan;
            case string_hash("light_magenta"): return light_magenta;
            case string_hash("light_orange"): return light_orange;
            case string_hash("light_mi32"): return light_mint;
            case string_hash("light_violet"): return light_violet;
            case string_hash("light_crimson"): return light_crimson;
            case string_hash("light_lime"): return light_lime;
            case string_hash("light_azure"): return light_azure;

            case string_hash("gray"): return gray;
            case string_hash("orange"): return orange;
            case string_hash("mi32"): return mint;
            case string_hash("violet"): return violet;
            case string_hash("crimson"): return crimson;
            case string_hash("lime"): return lime;
            case string_hash("azure"): return azure;

            case string_hash("half_red"): return half_red;
            case string_hash("half_green"): return half_green;
            case string_hash("half_blue"): return half_blue;
            case string_hash("half_yellow"): return half_yellow;
            case string_hash("half_cyan"): return half_cyan;
            case string_hash("half_magenta"): return half_magenta;

            case string_hash("dark_gray"): return dark_gray;
            case string_hash("dark_red"): return dark_red;
            case string_hash("dark_green"): return dark_green;
            case string_hash("dark_blue"): return dark_blue;
            case string_hash("dark_yellow"): return dark_yellow;
            case string_hash("dark_cyan"): return dark_cyan;
            case string_hash("dark_magenta"): return dark_magenta;
            case string_hash("dark_orange"): return dark_orange;
            case string_hash("dark_mi32"): return dark_mint;
            case string_hash("dark_violet"): return dark_violet;
            case string_hash("dark_crimson"): return dark_crimson;
            case string_hash("dark_lime"): return dark_lime;
            case string_hash("dark_azure"): return dark_azure;

            default: return transparent_black;
        }
    }
}
