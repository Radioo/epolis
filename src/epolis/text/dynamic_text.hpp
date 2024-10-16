#pragma once

#include <unordered_map>
#include <wx/wx.h>
#include <string>

namespace epolis::text {
    class dynamic_text {
    public:
        void refresh_text();
        void add_button(wxButton* button);
        void add_static_text(wxStaticText *text);
        void clear_static_text();

        void add_choice(wxChoice *choice, const wxArrayString &values);

    private:
        std::unordered_map<wxButton*, std::string> buttons;
        std::unordered_map<wxStaticText*, std::string> static_texts;
        std::unordered_map<wxChoice*, wxArrayString> choices;
    };
}