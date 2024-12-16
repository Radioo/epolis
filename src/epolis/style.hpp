#pragma once

#include <cstdint>

namespace epolis {
    constexpr std::uint8_t MARGIN = 5;
    constexpr float SCALE = 1.5;
    constexpr float BUTTON_FONT = 12;

    constexpr float IMAGE_WIDTH = 200;
    constexpr float IMAGE_HEIGHT = 200;

    enum class menu_item : std::uint8_t {
        language = 1,
        load_image_1 = 2,
        save_right_image_button = 8,
        fill_holes = 11,
        clear_borders = 12,
        operations = 14,
        run_button = 15,
        previous_step = 16,
        start = 17,
        stop = 18,
        next_step = 19,
    };
}