#pragma once

#include <wx/wx.h>
#include <cstdint>

namespace epolis::frame {
    class main final : public wxFrame {
    public:
        main();

    private:
        enum class menu_item : std::uint8_t {
            language = 1,
        };

        void on_change_language(const wxCommandEvent& event);
    };
}