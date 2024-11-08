#include <filesystem>
#include <wx/wrapsizer.h>

#include "epolis/frame/main.hpp"
#include "epolis/text/text.hpp"
#include "epolis/style.hpp"

epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS", wxDefaultPosition, wxSize(1280, 720)) {
    app_panel = new wxPanel(this, wxID_ANY);

    app_panel->SetFont(app_panel->GetFont().Scale(SCALE));

    auto* outer_sizer = new wxBoxSizer(wxVERTICAL);
    auto* top_sizer = new wxBoxSizer(wxVERTICAL);
    auto* top_menu_sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* main_sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* main_left_sizer = new wxBoxSizer(wxVERTICAL);
    auto* button_hSizer = new wxWrapSizer(wxHORIZONTAL);
    left_sizer = new wxBoxSizer(wxVERTICAL);

    const wxArrayString languages = {
        "English",
        "Polish",
    };

    operations = {
        {"Fill holes", {
            "Step 1 Fill",
            "Step 2 Fill",
            "Step 3 Fill"
        }},
        {"Clean borders", {
            "Step 1 Clean",
            "Step 2 Clean"
        }}
    };


    auto* language_choice = new wxChoice(app_panel, static_cast<int>(menu_item::language), wxDefaultPosition, wxDefaultSize, languages, 0);
    add_choice(language_choice,languages);
    Bind(wxEVT_CHOICE, &main::on_change_language, this, static_cast<int>(menu_item::language));
    language_choice->SetSelection(0);

    auto* operation_choice = new wxChoice(app_panel, static_cast<int>(menu_item::operations), wxDefaultPosition, wxDefaultSize, get_operation_names(), 0);
    add_choice(operation_choice,get_operation_names());
    Bind(wxEVT_CHOICE, &main::on_change_operation, this, static_cast<int>(menu_item::operations));
    operation_choice->SetSelection(0);

    auto* load_image_1_button = new wxButton(app_panel, static_cast<int>(menu_item::load_image_1), "Load Image");
    add_button(load_image_1_button);
    Bind(wxEVT_BUTTON, &main::on_load_image, this, static_cast<int>(menu_item::load_image_1));

    auto* title_sizer = new wxBoxSizer(wxVERTICAL);

    auto* run_button = new wxButton(app_panel, static_cast<int>(menu_item::run_button), "Run");
    add_button(run_button);
    Bind(wxEVT_BUTTON, &main::on_run_button, this, static_cast<int>(menu_item::run_button));

    auto* slider_sizer = new wxBoxSizer(wxHORIZONTAL);

    auto* slider_text = new wxStaticText(app_panel, wxID_ANY, "Animation speed");
    add_static_text(slider_text);

    timer_slider = new wxSlider(app_panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL | wxSL_LABELS);
    timer_slider->SetTickFreq(10);
    timer_slider->Bind(wxEVT_SCROLL_CHANGED, &main::on_timer_slider, this);

    slider_sizer->Add(slider_text, 0, 0, 5);
    slider_sizer->Add(timer_slider, 0, 0, 5);

    auto* title_text = new wxStaticText(app_panel, wxID_ANY, "Morphological transformations");
    add_static_text(title_text);
    auto font = title_text->GetFont();
    font.SetWeight(wxFONTWEIGHT_BOLD);
    title_text->SetFont(font);

    title_sizer->Add(title_text, 0, wxALIGN_CENTER_HORIZONTAL, 5);

    auto* save_image_button = new wxButton(app_panel, static_cast<int>(menu_item::save_right_image_button), "Save image");
    add_button(save_image_button);
    Bind(wxEVT_BUTTON, &main::on_save_image_button, this, static_cast<int>(menu_item::save_right_image_button));
    Bind(wxEVT_TIMER, &main::animate_marker_reconstruction, this);


    top_sizer->Add(title_sizer, 1, wxCENTER, 5);

    top_menu_sizer->Add(load_image_1_button, 0, wxALL, 5);
    top_menu_sizer->Add(language_choice, 0, wxTOP, 5);
    top_menu_sizer->Add(operation_choice, 0, wxALL, 5);
    top_menu_sizer->Add(run_button, 0, wxALL, 5);
    top_menu_sizer->Add(slider_sizer, 0, wxALL, 5);


    top_menu_sizer->AddStretchSpacer(1);
    top_menu_sizer->Add(save_image_button, 0, wxALL, 5);

    main_left_sizer->Add(button_hSizer, 0, wxALL, 0);

    images_sizer = new wxWrapSizer(wxHORIZONTAL, wxALIGN_CENTER_HORIZONTAL);

    main_left_sizer->AddStretchSpacer(1);
    main_left_sizer->Add(left_sizer, 1, wxEXPAND | wxALL, 5);
    main_left_sizer->AddStretchSpacer(1);

    main_sizer->Add(main_left_sizer, 1, wxEXPAND | wxALL, 5);

    auto* main_right_sizer = new wxBoxSizer(wxVERTICAL);
    main_right_sizer->AddStretchSpacer(1);
    main_right_sizer->Add(images_sizer, 1, wxEXPAND | wxALL, 5);
    main_right_sizer->AddStretchSpacer(1);
    main_sizer->Add(main_right_sizer, 1, wxEXPAND | wxALL, 5);

    app_panel->SetSizer(outer_sizer);

    timer.SetOwner(this);

    Centre(wxBOTH);

    outer_sizer->Add(top_sizer, 0, wxEXPAND, 5);
    outer_sizer->Add(top_menu_sizer, 0, wxEXPAND, 5);
    outer_sizer->Add(main_sizer, 0, wxEXPAND, 5);

    wxTopLevelWindow::Maximize(true);
    initialise_layout();

    wxCommandEvent event(wxEVT_CHOICE, operation_choice->GetId());
    event.SetInt(operation_choice->GetSelection());  // Set the selection index
    event.SetString(operation_choice->GetString(operation_choice->GetSelection()));
    on_change_operation(event);
}

void epolis::frame::main::on_run_button(const wxCommandEvent& event) {
    if (input_image.empty()) {
        return;
    }

    image_output->SetBitmap(get_empty_bitmap());
    for (const auto& step : step_images) {
        step.second->SetBitmap(get_empty_bitmap());
    }

    if (operation == "Clean borders") {
        on_clean_borders();
    } else if (operation == "Fill holes") {
        on_fill_holes();
    }

    app_panel->Layout();
}

void epolis::frame::main::on_timer_slider(const wxCommandEvent& event) {
    if(timer.IsRunning()) {
        std::cout << timer_slider->GetValue() << std::endl;
        timer.Stop();
        timer.Start(timer_slider->GetValue(), wxTIMER_CONTINUOUS);
    }
}


void epolis::frame::main::on_change_language(const wxCommandEvent& event) {
    const auto lang = static_cast<text::lang>(event.GetSelection());
    text::text::set_language(lang);
    refresh_text();
    app_panel->Layout();
}

void epolis::frame::main::initialise_layout() {
    auto* input_image_sizer = new wxBoxSizer(wxVERTICAL);
    auto* input_image_title = new wxStaticText(app_panel, wxID_ANY, "Input Image");
    add_static_text(input_image_title);
    image_input_1 = new wxStaticBitmap(app_panel, wxID_ANY, get_empty_bitmap());
    input_image_sizer->Add(input_image_title, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    input_image_sizer->Add(image_input_1, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    box_map["Input Image"] = input_image_sizer;

    for (const auto& operation : operations) {
        for (const auto& step: operation.second) {
            auto* step_image_sizer = new wxBoxSizer(wxVERTICAL);
            auto* step_image_title = new wxStaticText(app_panel, wxID_ANY, step.ToStdString());
            add_static_text(step_image_title);
            step_images[step.ToStdString()] = new wxStaticBitmap(app_panel, wxID_ANY, get_empty_bitmap());
            step_image_sizer->Add(step_image_title, 0, wxALIGN_CENTER_HORIZONTAL, 5);
            step_image_sizer->Add(step_images[step.ToStdString()], 0, wxALIGN_CENTER_HORIZONTAL, 5);
            box_map[step.ToStdString()] = step_image_sizer;
        }
    }

    auto* output_image_sizer = new wxBoxSizer(wxVERTICAL);
    auto* image_output_title = new wxStaticText(app_panel, wxID_ANY, "Output");
    add_static_text(image_output_title);
    image_output = new wxStaticBitmap(app_panel, wxID_ANY, get_empty_bitmap());
    output_image_sizer->Add(image_output_title, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    output_image_sizer->Add(image_output, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    box_map["Output"] = output_image_sizer;
}

void epolis::frame::main::on_change_operation(const wxCommandEvent& event) {
    timer.Stop();
    operation_function.animate_marker_reconstruction(true);
    input_image.release();
    image_input_1->SetBitmap(get_empty_bitmap());
    image_output->SetBitmap(get_empty_bitmap());
    for (const auto& box: step_images) {
        box.second->SetBitmap(get_empty_bitmap());
    }
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

    left_sizer->Add(box_map["Input Image"], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    box_map["Input Image"]->Show(true);
    for (const auto& step : operations[operation]) {
        images_sizer->Add(box_map[step.ToStdString()], 1, wxALL | wxEXPAND, 5);
        box_map[step.ToStdString()]->Show(true);
    }
    images_sizer->Add(box_map["Output"], 1, wxALL | wxEXPAND, 5);
    box_map["Output"]->Show(true);

    refresh_text();
    app_panel->Layout();
}

void epolis::frame::main::on_load_image(const wxCommandEvent& event) {
    const auto item = static_cast<menu_item>(event.GetId());

    auto* dialog = new wxFileDialog(
        this,
        "Select an image",
        (std::filesystem::current_path() / "input").string(),
        "",
        "Supported formats (*.png;*.jpg;*.bmp;*.webp;*.tif)|*.png;*.jpg;*.bmp;*.webp;*.tif",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if(dialog->ShowModal() == wxID_CANCEL) {
        return;
    }

    const auto path = dialog->GetPath();
    input_image = imread(std::string(path), cv::IMREAD_COLOR);

    // Convert BGR to RGB
    cv::Mat rgb_image;
    cv::cvtColor(input_image, rgb_image, cv::COLOR_BGR2RGB);

    const wxImage wx_image(rgb_image.cols, rgb_image.rows, rgb_image.data, true);
    image_input_1->SetBitmap(wxBitmap(wx_image));

    app_panel->Layout();
}

void epolis::frame::main::on_fill_holes() {
    operation_function.animate_marker_reconstruction(true);
    operation_function.fill_holes(input_image);

    step_images["Step 1 Fill"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_input_image_binary())); //binaryzacja
    step_images["Step 2 Fill"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_inverted_image())); //negacja
    step_images["Step 3 Fill"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_destination())); //markery
    image_output->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_result())); //wynik koncowy

    app_panel->Layout();
}

void epolis::frame::main::on_clean_borders() {
    timer.Stop();
    operation_function.animate_marker_reconstruction(true);
    operation_function.clean_borders(input_image);

    step_images["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_marker_animation_frame()));

    step_images["Step 1 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_input_image_binary())); // binaryzacja
    app_panel->Layout();
    timer.Start(timer_slider->GetValue(), wxTIMER_CONTINUOUS);
}

void epolis::frame::main::animate_marker_reconstruction(wxTimerEvent &event) {
    if(operation_function.animate_marker_reconstruction()) {
        image_output->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_destination()));
        timer.Stop();
    }

    step_images["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_animation_frame()));
    // app_panel->Layout();
}

wxArrayString epolis::frame::main::get_operation_names() {
    wxArrayString operation_names;

    for (const auto& operation : operations) {
        operation_names.Add(operation.first);
    }

    return operation_names;
}

void epolis::frame::main::on_save_image_button(const wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save Image",
                                (std::filesystem::current_path() / "output").string(),
                                "",
                                "Supported formats (*.png)|*.png",
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxString filePath = saveFileDialog.GetPath();

    if (!filePath.Lower().EndsWith(".png")) {
        filePath += ".png";
    }

    const wxBitmap bitmap = image_output->GetBitmap();

    if (!bitmap.IsOk()) {
        wxMessageBox("Failed to get image from wxStaticBitmap", "Error", wxICON_ERROR);
        return;
    }

    const wxImage image = bitmap.ConvertToImage();

    if (!image.IsOk()) {
        wxMessageBox("Invalid image", "Error", wxICON_ERROR);
        return;
    }

    const int width = image.GetWidth();
    const int height = image.GetHeight();

    cv::Mat mat_image(height, width, CV_8UC3, image.GetData());

    cvtColor(mat_image, mat_image, cv::COLOR_RGB2BGR);

    if (imwrite(std::string(filePath.mb_str()), mat_image)) {
        wxMessageBox("Image saved successfully", "Success", wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to save image", "Error", wxICON_ERROR);
    }
}

wxBitmap epolis::frame::main::mat_to_bitmap(const cv::Mat& image) {
    const wxImage wx_image(image.cols, image.rows, image.data, true);
    auto bitmap = wxBitmap(wx_image);
    return bitmap;
}

wxBitmap epolis::frame::main::mat_to_bitmap_greyscale(const cv::Mat& image) {
    cv::Mat grayscale_rgb;

    if (image.channels() == 1) {
        cv::cvtColor(image, grayscale_rgb, cv::COLOR_GRAY2BGR);
    } else {
        grayscale_rgb = image;
    }
    const wxImage wx_image(grayscale_rgb.cols, grayscale_rgb.rows, grayscale_rgb.data, true);
    auto bitmap = wxBitmap(wx_image);
    return bitmap;
}

wxBitmap epolis::frame::main::get_empty_bitmap() {
    auto bitmap = wxBitmap(256, 256);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    return bitmap;
}
