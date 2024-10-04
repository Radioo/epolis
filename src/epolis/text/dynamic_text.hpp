#pragma once

#include <unordered_map>
#include <wx/wx.h>
#include <string>

namespace epolis::text {
    class dynamic_text {
    public:
        void refresh_text();
        void add_button(wxButton* button);

    private:
        std::unordered_map<wxButton*, std::string> buttons;
    };
}