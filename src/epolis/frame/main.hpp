#pragma once

#include <wx/wx.h>
#include <cstdint>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

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
            erosion = 4,
            dilatation = 5,
            opening = 6,
            closing = 7,
        };

        enum class image_input : std::uint8_t {
            image_1 = 1,
            image_2 = 2,
        };

        wxStaticBitmap* selected_input = nullptr;
        wxStaticBitmap* image_input_1;
        wxStaticBitmap* image_input_2;
        wxStaticBitmap* image_output;

        static wxBitmap get_empty_bitmap();
        static cv::Mat bitmap_to_mat(const wxStaticBitmap* image);
        static wxBitmap mat_to_bitmap(const cv::Mat& image);

        void on_change_language(const wxCommandEvent& event);
        void on_load_image(const wxCommandEvent& event);
        void on_select_image(const wxMouseEvent& event);
        void on_erosion(const wxCommandEvent& event);
        void on_dilation(const wxCommandEvent& event);
        void on_opening(const wxCommandEvent& event);
        void on_closing(const wxCommandEvent& event);

        void select_image(image_input image);

    };
}