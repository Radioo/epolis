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
            save_right_image_button = 8,
            copy_right_image_to_left_top_button = 9,
            copy_right_image_to_left_bottom_button = 10,
            kernel_size_slider = 11,
            morph_shape = 12,
        };

        enum class image_input : std::uint8_t {
            image_1 = 1,
            image_2 = 2,
        };

        wxStaticBitmap* selected_input = nullptr;
        wxStaticBitmap* image_input_1;
        wxStaticBitmap* image_input_2;
        wxStaticBitmap* image_output;

        std::uint8_t kernel_size_value = 1;
        cv::MorphShapes morph_shape = cv::MORPH_RECT;

        static wxBitmap get_empty_bitmap();
        static cv::Mat bitmap_to_mat(const wxStaticBitmap* image);
        static wxBitmap mat_to_bitmap(const cv::Mat& image);

        void on_shape_change(const wxCommandEvent& event);
        void on_change_language(const wxCommandEvent& event);
        void on_load_image(const wxCommandEvent& event);
        void on_select_image(const wxMouseEvent& event);
        void on_erosion(const wxCommandEvent& event);
        void on_dilation(const wxCommandEvent& event);
        void on_opening(const wxCommandEvent& event);
        void on_closing(const wxCommandEvent& event);
        void on_save_right_image_button(const wxCommandEvent& event);
        void on_copy_right_image_to_left_top_button(const wxCommandEvent& event);
        void on_copy_right_image_to_left_bottom_button(const wxCommandEvent& event);

        void select_image(image_input image);

    };
}