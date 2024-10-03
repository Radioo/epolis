#pragma once

#include <unordered_map>
#include <string>

#include "epolis/text/lang.hpp"

namespace epolis::text {
    class text {
    public:
        static void load_text();
        static std::string& translate(const std::string& source);
        static void set_language(lang language);

    private:
        static inline auto current_lang = lang::en;
        static inline std::unordered_map<std::string, std::unordered_map<lang, std::string>> text_map;
    };
}