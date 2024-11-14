#pragma once

#include <wx/wx.h>

#include "lang.hpp"

namespace epolis::text {
    class bitmap {
    public:
        void load_bitmap();
        void change_language();

        wxBitmap& get_bitmap(int selection);

        lang get_lang();

    private:
        std::vector<wxBitmap> bitmap_vector;

        lang language = lang::en;

        lang string_to_lang(const std::string& str);
    };
};



