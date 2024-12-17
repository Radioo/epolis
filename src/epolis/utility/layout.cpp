#include "epolis/utility/layout.h"

#include <epolis/style.hpp>

namespace epolis::utility {
    layout::layout() {
        operations = {
            {
                "Fill holes", {
                    "Step 1 Fill",
                    "Step 2 Fill",
                    "Step 3 Fill"
                }
            },
            {
                "Clean borders", {
                    "Step 1 Clean",
                    "Step 2 Clean"
                }
            }
        };
    }

    void layout::initialise_layout(wxPanel* panel) {
        auto *outer_sizer = new wxBoxSizer(wxVERTICAL);
        auto *main_sizer = new wxBoxSizer(wxHORIZONTAL);
        auto *main_left_sizer = new wxBoxSizer(wxVERTICAL);
        auto *button_hSizer = new wxWrapSizer(wxHORIZONTAL);
        auto *top_sizer = new wxBoxSizer(wxVERTICAL);

        left_sizer = new wxBoxSizer(wxVERTICAL);
        top_menu_sizer = new wxBoxSizer(wxHORIZONTAL);

        language_bitmap.load_bitmap();

        bitmapButton = new wxBitmapButton(panel, static_cast<int>(menu_item::language), language_bitmap.get_bitmap(0), wxDefaultPosition, wxDefaultSize);

        auto *operation_slider = new wxBoxSizer(wxHORIZONTAL);

        auto *operation_text = new wxStaticText(panel, wxID_ANY, "Select operation");
        add_static_text(operation_text);

        operation_choice = new wxChoice(panel, static_cast<int>(menu_item::operations), wxDefaultPosition,
                                          wxDefaultSize, get_operation_names(), 0);
        add_choice(operation_choice, get_operation_names());
        operation_choice->SetSelection(0);

        operation_slider->Add(operation_text, 0, wxALL, 5);
        operation_slider->Add(operation_choice, 0, 0, 5);

        auto *load_image_1_button = new wxButton(panel, static_cast<int>(menu_item::load_image_1), "Load Image");
        add_button(load_image_1_button);

        auto *title_sizer = new wxBoxSizer(wxVERTICAL);

        auto *run_button = new wxButton(panel, static_cast<int>(menu_item::run_button), "Run");
        add_button(run_button);

        slider_sizer = new wxBoxSizer(wxHORIZONTAL);

        auto *slider_text = new wxStaticText(panel, wxID_ANY, "Animation speed");
        add_static_text(slider_text);

        timer_slider = new TriangularSlider(panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxSize(200, 40));

        slider_sizer->Add(slider_text, 0, wxALL, 5);
        slider_sizer->Add(timer_slider, 0, 0, 5);
        slider_sizer->Show(false);

        auto *title_text = new wxStaticText(panel, wxID_ANY, "Morphological transformations");
        add_static_text(title_text);
        auto font = title_text->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        title_text->SetFont(font);

        title_sizer->Add(title_text, 0, wxALIGN_CENTER_HORIZONTAL, 5);

        save_image_button = new wxButton(panel, static_cast<int>(menu_item::save_right_image_button), "Save image");
        add_button(save_image_button);

        save_image_button->Show(false);

        top_sizer->AddStretchSpacer(1);
        top_sizer->Add(title_sizer, 1, wxCENTER, 5);
        top_sizer->AddStretchSpacer(1);


        top_menu_sizer->Add(bitmapButton, 0, wxALL, 5);
        top_menu_sizer->Add(load_image_1_button, 0, wxALL, 5);
        top_menu_sizer->Add(operation_slider, 0, wxALL, 5);
        top_menu_sizer->Add(run_button, 0, wxALL, 5);

        main_left_sizer->Add(button_hSizer, 0, wxALL, 0);

        images_sizer = new wxWrapSizer(wxHORIZONTAL, wxALIGN_CENTER_HORIZONTAL);

        main_left_sizer->Add(left_sizer, 1, wxEXPAND | wxALL, 5);

        main_sizer->Add(main_left_sizer, 0, wxEXPAND | wxALL, 15);

        auto *main_right_sizer = new wxBoxSizer(wxVERTICAL);
        main_right_sizer->AddStretchSpacer(2);
        main_right_sizer->Add(images_sizer, 1, wxEXPAND | wxALL, 0);
        main_right_sizer->AddStretchSpacer(3);
        main_sizer->Add(main_right_sizer, 1, wxEXPAND | wxALL, 5);

        panel->SetSizer(outer_sizer);

        outer_sizer->Add(top_sizer, 0, wxEXPAND, 5);
        outer_sizer->Add(top_menu_sizer, 0, wxEXPAND, 5);
        outer_sizer->Add(main_sizer, 0, wxEXPAND, 5);
    }

    void layout::initialise_operation_bitmaps(wxPanel* panel) {
        top_menu_sizer->Add(slider_sizer, 0, wxALL, 5);
        top_menu_sizer->AddStretchSpacer(1);
        top_menu_sizer->Add(save_image_button, 0, wxALL, 5);
        slider_sizer->Show(true);
        save_image_button->Show(true);

        initial_run = true;

        auto* input_image_sizer = new wxBoxSizer(wxVERTICAL);
        auto* input_image_title = new wxStaticText(panel, wxID_ANY, "Input Image");
        add_static_text(input_image_title);
        image_input_1 = new wxStaticBitmap(panel, wxID_ANY, get_empty_bitmap());
        input_image_sizer->Add(input_image_title, 0, wxALIGN_CENTER_HORIZONTAL, 5);
        input_image_sizer->Add(image_input_1, 0, wxALIGN_CENTER_HORIZONTAL, 5);
        box_map["Input Image"] = input_image_sizer;

        step_control_sizer = new wxBoxSizer(wxHORIZONTAL);

        auto* previous_step_button = new wxButton(panel, static_cast<int>(menu_item::previous_step), "Previous step", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        wxFont font = previous_step_button->GetFont();
        font.SetPointSize(BUTTON_FONT);
        previous_step_button->SetFont(font);
        add_button(previous_step_button);

        stop_button = new wxButton(panel, static_cast<int>(menu_item::stop), "Pause", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        stop_button->SetFont(font);
        add_button(stop_button);

        start_button = new wxButton(panel, static_cast<int>(menu_item::start), "Start", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        start_button->SetFont(font);
        add_button(start_button);
        start_button->Show(false);

        auto* next_step_button = new wxButton(panel, static_cast<int>(menu_item::next_step), "Next step", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        next_step_button->SetFont(font);
        add_button(next_step_button);

        step_control_sizer->Add(previous_step_button, 0, wxALL, 5);
        step_control_sizer->Add(stop_button, 0, wxALL, 5);
        step_control_sizer->Add(next_step_button, 0, wxALL, 5);

        for (const auto& operation : operations) {
            for (const auto& step: operation.second) {
                auto* step_image_sizer = new wxBoxSizer(wxVERTICAL);

                auto* step_image_title = new wxStaticText(panel, wxID_ANY, step.ToStdString());
                add_static_text(step_image_title);

                step_images[step.ToStdString()] = new wxStaticBitmap(panel, wxID_ANY, get_empty_bitmap());

                step_image_sizer->Add(step_image_title, 0, wxALIGN_CENTER_HORIZONTAL, 5);
                step_image_sizer->Add(step_images[step.ToStdString()], 0, wxALIGN_CENTER_HORIZONTAL, 5);
                if(step.ToStdString() == "Step 2 Clean" ) {
                    step_image_sizer->Add(step_control_sizer, 0, wxALIGN_CENTER_HORIZONTAL, 5);
                }

                box_map[step.ToStdString()] = step_image_sizer;
            }
        }

        auto* output_image_sizer = new wxBoxSizer(wxVERTICAL);
        auto* image_output_title = new wxStaticText(panel, wxID_ANY, "Output");
        add_static_text(image_output_title);
        image_output = new wxStaticBitmap(panel, wxID_ANY, get_empty_bitmap());
        output_image_sizer->Add(image_output_title, 0, wxALIGN_CENTER_HORIZONTAL, 5);
        output_image_sizer->Add(image_output, 0, wxALIGN_CENTER_HORIZONTAL, 5);
        box_map["Output"] = output_image_sizer;

    }

    void layout::on_change_operation(const wxCommandEvent& event) {
        clear_step_images();
    for (const auto& box: box_map) {
        box.second->Show(false);
    }

    for (int i = left_sizer->GetItemCount() - 1; i >= 0; i--) {
        left_sizer->Detach(i);
    }

    for (int i = images_sizer->GetItemCount() - 1; i >= 0; i--) {
        images_sizer->Detach(i);
    }

    operation = get_operation_names().Item(event.GetSelection());
    static wxString last_operation = "";

    if(operation == "Clean borders" && !initial_run && operation != last_operation) {
        top_menu_sizer->Remove(4);
        top_menu_sizer->Detach(save_image_button);
         top_menu_sizer->Add(slider_sizer, 0, wxALL, 5);
         top_menu_sizer->AddStretchSpacer(1);
         top_menu_sizer->Add(save_image_button, 0, wxALL, 5);
         slider_sizer->Show(true);
         save_image_button->Show(true);

        step_control_sizer->Detach(1);
        start_button->Show(false);
        step_control_sizer->Insert(1,stop_button,wxEXPAND, 5);
        stop_button->Show(true);
    }
    else if(operation == "Fill holes" && !initial_run && operation != last_operation) {
        top_menu_sizer->Detach(slider_sizer);
        slider_sizer->Show(false);
    }
    else {
        initial_run = false;
    }
    last_operation = operation;

        //left_sizer->Add(box_map["Input Image"], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        images_sizer->Add(box_map["Input Image"], 1, wxALL | wxEXPAND, 5);
        box_map["Input Image"]->Show(true);
    for (const auto& step : operations[operation]) {
        images_sizer->Add(box_map[step.ToStdString()], 1, wxALL | wxEXPAND, 5);
        box_map[step.ToStdString()]->Show(true);
    }
        images_sizer->Add(box_map["Output"], 1, wxALL | wxEXPAND, 5);
        box_map["Output"]->Show(true);

    }

    void layout::on_animation_pause() {
        step_control_sizer->Detach(1);
        stop_button->Show(false);
        step_control_sizer->Insert(1,start_button,wxEXPAND, 5);
        start_button->Show(true);
    }

    void layout::on_animation_resume() {
        step_control_sizer->Detach(1);
        start_button->Show(false);
        step_control_sizer->Insert(1,stop_button,wxEXPAND, 5);
        stop_button->Show(true);
    }

    wxString layout::on_run_button() {
        image_output->SetBitmap(get_empty_bitmap());
        for (const auto& step : step_images) {
            step.second->SetBitmap(get_empty_bitmap());
        }
        if(operation == "Clean borders")
            on_animation_resume();

        return operation;
    }

    wxArrayString layout::get_operation_names() {
        wxArrayString operation_names;

        for (const auto& operation : operations) {
            operation_names.Add(operation.first);
        }

        return operation_names;
    }

    wxCommandEvent layout::get_initial_selection() {
        wxCommandEvent event(wxEVT_CHOICE, operation_choice->GetId());
        event.SetInt(operation_choice->GetSelection()); // Set the selection index
        event.SetString(operation_choice->GetString(operation_choice->GetSelection()));
        return event;
    }

    wxBitmap layout::get_empty_bitmap() {
        auto bitmap = wxBitmap(IMAGE_WIDTH, IMAGE_HEIGHT);
        wxMemoryDC dc(bitmap);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        return bitmap;
    }

    void layout::clear_step_images() {
        for (const auto& box: step_images) {
            box.second->SetBitmap(get_empty_bitmap());
        }
        image_output->SetBitmap(get_empty_bitmap());
    }

    text::lang layout::on_change_language() {
        language_bitmap.change_language();
        bitmapButton->SetBitmap(language_bitmap.get_bitmap(static_cast<int>(language_bitmap.get_lang())));
        return language_bitmap.get_lang();
    }

    void layout::on_load_image(const wxImage& input_image) {
        clear_step_images();
        image_input_1->SetBitmap(wxBitmap(input_image));
        if(operation == "Clean borders")
        on_animation_resume();
    }

    TriangularSlider* layout::get_timer_slider() {
        return timer_slider;
    }

    wxStaticBitmap* layout::get_image_output() {
        return image_output;
    }

    std::unordered_map<std::string, wxStaticBitmap *>& layout::get_step_images() {
        return step_images;
    }

}

