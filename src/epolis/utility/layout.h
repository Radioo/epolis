#pragma once

#include <map>
#include <wx/wx.h>

#include "epolis/frame/custom/triangle_slider.hpp"
#include <epolis/text/dynamic_text.hpp>
#include <wx/wrapsizer.h>

#include "epolis/text/bitmap.h"

namespace epolis::utility {
    class layout : public text::dynamic_text {
    public:
        layout();

        void initialise_layout(wxPanel* panel);
        void initialise_operation_bitmaps(wxPanel* panel);
        void on_change_operation(const wxCommandEvent& event);
        void on_animation_pause();
        void on_animation_resume();
        void clear_step_images();
        void on_load_image(const wxImage& input_image);

        text::lang on_change_language();
        wxString on_run_button();

        TriangularSlider* get_timer_slider();

        wxArrayString get_operation_names();

        wxCommandEvent get_initial_selection();

        wxStaticBitmap* get_image_output();

        std::unordered_map<std::string, wxStaticBitmap*>& get_step_images();
    private:
        bool initial_run;
        bool pause;
        bool is_running;

        wxString operation;

        wxBoxSizer* left_sizer;
        wxBoxSizer* top_menu_sizer;
        wxBoxSizer* slider_sizer;
        wxBoxSizer* step_control_sizer;

        wxWrapSizer* images_sizer;

        wxBitmapButton* bitmapButton;
        wxButton* save_image_button;
        wxButton* stop_button;
        wxButton* start_button;

        wxChoice* operation_choice;

        TriangularSlider* timer_slider;

        wxStaticBitmap* image_input_1;
        wxStaticBitmap* image_output;

        text::bitmap language_bitmap;

        std::map<wxString, wxArrayString> operations;
        std::unordered_map<std::string, wxBoxSizer*> box_map;
        std::unordered_map<std::string, wxStaticBitmap*> step_images;

        static wxBitmap get_empty_bitmap();
    };
}

