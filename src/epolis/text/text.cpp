#include <csv.hpp>
#include <filesystem>

#include "epolis/text/text.hpp"

void epolis::text::text::load_text() {
    if(!std::filesystem::exists("text.csv")) {
        throw std::runtime_error("text.csv does not exist");
    }

    for(csv::CSVReader reader("text.csv"); const auto& row : reader) {
        text_map[row["source"].get<std::string>()] = {
            {lang::en, row["en"].get<std::string>()},
            {lang::pl, row["pl"].get<std::string>()},
        };
    }
}

std::string& epolis::text::text::translate(const std::string& source) {
    if(!text_map.contains(source)) {
        throw std::runtime_error("Translation for `" + std::string(source) + "` does not exist");
    }

    return text_map[source][current_lang];
}

void epolis::text::text::set_language(lang language) {
    current_lang = language;
}
