#pragma once

#include <wx/wx.h>
#include <cstdint>
#include <map>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "epolis/text/dynamic_text.hpp"
#include "epolis/utility/operations.h"
#include "epolis/utility/layout.h"

#include "epolis/frame/custom/triangle_slider.hpp"

class wxWrapSizer;

namespace epolis::frame {
    class main final : public wxFrame {
    public:
        main();

    private:
        bool pause;
        bool is_running;

        wxPanel* app_panel;

        cv::Mat input_image;

        utility::operations operation_function;
        utility::layout layout;

        wxTimer timer;

        static wxBitmap mat_to_bitmap_greyscale(const cv::Mat &image);

        void animate_marker_reconstruction(wxTimerEvent& event);
        void on_fill_holes();
        void on_clean_borders();
        void on_change_language(const wxCommandEvent& event);
        void on_change_operation(const wxCommandEvent& event);
        void on_load_image(const wxCommandEvent& event);
        void on_save_image_button(const wxCommandEvent& event);
        void on_run_button(const wxCommandEvent& event);
        void on_timer_slider(const wxCommandEvent& event);
        void on_previous_step(const wxCommandEvent& event);
        void on_animation_pause(const wxCommandEvent& event);
        void on_animation_resume(const wxCommandEvent& event);
        void on_next_step(const wxCommandEvent& event);
    };
}
