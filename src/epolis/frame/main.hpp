#pragma once

#include <wx/wx.h>
#include <cstdint>
#include <map>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "epolis/text/dynamic_text.hpp"
#include "epolis/utility/operations.h"

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
            toggle_animation = 16,
            struct_shapes = 17,
        };

        wxPanel* app_panel;
        wxBoxSizer* left_sizer;
        wxSlider* timer_slider;
        wxCheckBox* animation_toggle;

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

        wxString operation;

        wxTimer timer;

        cv::MorphShapes morph_shape = cv::MORPH_RECT;

        bool animate = false;

        static wxBitmap get_empty_bitmap();
        static cv::Mat bitmap_to_mat(const wxStaticBitmap* image);
        static cv::Mat bitmap_to_mat_grayscale(const wxStaticBitmap *image);
        static wxBitmap mat_to_bitmap(const cv::Mat& image);
        static wxBitmap mat_to_bitmap_greyscale(const cv::Mat &image);

        void initialise_layout();
        void animate_marker_reconstruction(wxTimerEvent& event);
        void on_fill_holes();
        void on_clean_borders();
        wxArrayString get_operation_names();
        void on_toggle_animation(wxCommandEvent& event);
        void on_change_struct_shape(const wxCommandEvent& event);
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
