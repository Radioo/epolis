#pragma once

#include <wx/wx.h>
#include <cstdint>
#include <map>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "epolis/text/dynamic_text.hpp"
#include "epolis/utility/operations.h"
#include "epolis/text/bitmap.h"

class wxWrapSizer;

namespace epolis::frame {
    class main final : public wxFrame, public text::dynamic_text {
    public:
        main();

    private:
        enum class menu_item : std::uint8_t {
            language = 1,
            load_image_1 = 2,
            save_right_image_button = 8,
            fill_holes = 11,
            clear_borders = 12,
            operations = 14,
            run_button = 15,
        };

        bool initial_run;

        wxPanel* app_panel;
        wxBoxSizer* left_sizer;
        wxBoxSizer* top_menu_sizer;
        wxBoxSizer* slider_sizer;
        wxSlider* timer_slider;

        cv::Mat input_image;
        cv::Mat input_image_binary;
        cv::Mat marker_animation_frame;

        wxStaticBitmap* selected_input = nullptr;
        wxStaticBitmap* image_input_1;
        wxStaticBitmap* image_output;

        wxWrapSizer* images_sizer;

        utility::operations operation_function;

        std::map<wxString, wxArrayString> operations;
        std::unordered_map<std::string, wxStaticBitmap*> step_images;
        std::unordered_map<std::string, wxBoxSizer*> box_map;

        wxBitmapButton* bitmapButton;
        wxButton* save_image_button;

        wxString operation;

        wxTimer timer;

        text::bitmap language_bitmap;

        static wxBitmap get_empty_bitmap();
        static cv::Mat bitmap_to_mat(const wxStaticBitmap* image);
        static cv::Mat bitmap_to_mat_grayscale(const wxStaticBitmap *image);
        static wxBitmap mat_to_bitmap(const cv::Mat& image);
        static wxBitmap mat_to_bitmap_greyscale(const cv::Mat &image);

        void initialise_layout();
        void animate_marker_reconstruction(wxTimerEvent& event);
        void on_fill_holes();
        void on_clean_borders();
        void clear_step_images();
        wxArrayString get_operation_names();
        void on_change_language(const wxCommandEvent& event);
        void on_change_operation(const wxCommandEvent& event);
        void on_load_image(const wxCommandEvent& event);
        void on_select_image(const wxMouseEvent& event);
        void on_clear_borders(const wxCommandEvent& event);
        void on_save_image_button(const wxCommandEvent& event);
        void on_run_button(const wxCommandEvent& event);
        void on_timer_slider(const wxCommandEvent& event);
    };
}
