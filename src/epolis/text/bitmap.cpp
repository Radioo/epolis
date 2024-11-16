#include "epolis/text/bitmap.h"

#include <filesystem>


void epolis::text::bitmap::load_bitmap() {
    wxImage::AddHandler(new wxPNGHandler());

    std::string folderPath = (std::filesystem::current_path() / "images").string();

    if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath)) {
        throw std::runtime_error("Images folder not found");
    }

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".png") {
            std::string filePath = entry.path().string();
            std::string filename = entry.path().stem().string();
            size_t underscorePos = filename.find('_');

            if (underscorePos != std::string::npos) {
                std::string prefix = filename.substr(0, underscorePos);

                wxBitmap bitmap(filePath, wxBITMAP_TYPE_PNG);
                wxImage image = bitmap.ConvertToImage();
                image.Rescale(50, 27, wxIMAGE_QUALITY_HIGH);
                wxBitmap resizedBitmap(image);

                if (bitmap.IsOk()) {
                    bitmap_vector.push_back(resizedBitmap);
                } else {
                    throw std::runtime_error("Failed to load image");
                }
            }
        }
    }
}

void epolis::text::bitmap::change_language() {
    if(language == lang::en) {
        language = lang::pl;
    }
    else {
        language = lang::en;
    }
}

wxBitmap& epolis::text::bitmap::get_bitmap(int selection) {
    return bitmap_vector[selection];
}

epolis::text::lang epolis::text::bitmap::get_lang() {
    return language;
}

