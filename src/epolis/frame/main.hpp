#pragma once

#include <wx/wx.h>
#include <cstdint>

#include "epolis/text/dynamic_text.hpp"

namespace epolis::frame {
    class main final : public wxFrame, public text::dynamic_text {
    public:
        main();

    private:
        enum class menu_item : std::uint8_t {
            language = 1,
            load_image_1 = 2,
            load_image_2 = 3,
        };

        void on_change_language(const wxCommandEvent& event);
        void on_load_image(const wxCommandEvent& event);
    };
}