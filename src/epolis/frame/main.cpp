#include <filesystem>

#include "epolis/frame/main.hpp"

#include <wx/wrapsizer.h>

#include "epolis/text/text.hpp"

epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS", wxDefaultPosition, wxSize(1280, 720)) {
    app_panel = new wxPanel(this, wxID_ANY);

    auto* outer_sizer = new wxBoxSizer(wxVERTICAL);

    auto* top_menu_sizer = new wxBoxSizer(wxHORIZONTAL);

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

    top_menu_sizer->Add(language_choice, 0, wxALL, 5);
    top_menu_sizer->Add(operation_choice, 0, wxALL, 5);
    top_menu_sizer->Add(load_image_1_button, 0, wxALL, 5);
    top_menu_sizer->Add(title_sizer, 1, wxALIGN_CENTER_VERTICAL, 5);
    top_menu_sizer->Add(save_image_button, 0, wxALL, 5);

    outer_sizer->Add(top_menu_sizer, 0, wxEXPAND, 5);

    images_sizer = new wxWrapSizer(wxHORIZONTAL, wxALIGN_CENTER_HORIZONTAL);

    outer_sizer->Add(images_sizer, 1, wxEXPAND, 5);

    app_panel->SetSizer(outer_sizer);

    timer.SetOwner(this);

    Centre(wxBOTH);

    initialise_layout();

    wxCommandEvent event(wxEVT_CHOICE, operation_choice->GetId());
    event.SetInt(operation_choice->GetSelection());  // Set the selection index
    event.SetString(operation_choice->GetString(operation_choice->GetSelection()));
    on_change_operation(event);
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
    image_input_1->SetBitmap(get_empty_bitmap());
    image_output->SetBitmap(get_empty_bitmap());
    for (const auto& box: step_images) {
        box.second->SetBitmap(get_empty_bitmap());
    }
    for (const auto& box: box_map) {
        box.second->Show(false);
    }
    for (int i = images_sizer->GetItemCount() - 1; i >= 0; i--) {
        images_sizer->Detach(i);
    }

    operation = get_operation_names().Item(event.GetSelection());

    images_sizer->Add(box_map["Input Image"], 1, wxALL | wxEXPAND, 5);
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
        "Supported formats (*.png;*.jpg;*bmp;*webp)|*.png;*.jpg;*.bmp;*.webp",
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

    if (operation == "Clean borders") {
        on_clean_borders();
    }
    else {
        on_fill_holes();

    }
    // app_panel->Layout();
}

void epolis::frame::main::on_fill_holes() {
    const cv::Mat source = input_image;
    cv::Mat destination,threshold,flood_fill,flood_fill2,inv,inv2,gray,marker;

    cvtColor(source, gray, cv::COLOR_BGR2GRAY);

    cv::threshold(gray,threshold, 128, 255, cv::THRESH_OTSU);
    bitwise_not(threshold, inv);

    cv::Mat mask = cv::Mat::zeros(inv.rows + 2, inv.cols + 2, CV_8UC1);
    cv::Mat mask2 = cv::Mat::zeros(threshold.rows + 2, threshold.cols + 2, CV_8UC1);
    flood_fill = inv.clone();
    flood_fill2 = threshold.clone();


    // cv::floodFill(flood_fill2, mask2, cv::Point(0, 0), cv::Scalar(0));
    for (int col = 0; col < flood_fill2.cols; ++col) {
        cv::floodFill(flood_fill2, mask2, cv::Point(col, 0), cv::Scalar(0));
        cv::floodFill(flood_fill2, mask2, cv::Point(col, flood_fill2.rows - 1), cv::Scalar(0));
    }
    for (int row = 0; row < flood_fill2.rows; ++row) {
        cv::floodFill(flood_fill2, mask2, cv::Point(0, row), cv::Scalar(0));
        cv::floodFill(flood_fill2, mask2, cv::Point(flood_fill2.cols - 1, row), cv::Scalar(0));
    }
    for (int col = 0; col < flood_fill.cols; ++col) {
        cv::floodFill(flood_fill, mask, cv::Point(col, 0), cv::Scalar(0));
        cv::floodFill(flood_fill, mask, cv::Point(col, flood_fill.rows - 1), cv::Scalar(0));
    }
    for (int row = 0; row < flood_fill.rows; ++row) {
        cv::floodFill(flood_fill, mask, cv::Point(0, row), cv::Scalar(0));
        cv::floodFill(flood_fill, mask, cv::Point(flood_fill.cols - 1, row), cv::Scalar(0));
    }

    destination = (threshold | flood_fill);
    cv::bitwise_or(flood_fill, flood_fill2, marker);
    cv::bitwise_xor(marker, destination, marker);
    step_images["Step 1 Fill"]->SetBitmap(mat_to_bitmap_greyscale(threshold)); //binaryzacja
    step_images["Step 2 Fill"]->SetBitmap(mat_to_bitmap_greyscale(inv)); //negacja
    step_images["Step 3 Fill"]->SetBitmap(mat_to_bitmap_greyscale(flood_fill)); //markery
    image_output->SetBitmap(mat_to_bitmap_greyscale(destination)); //wynik koncowy

    app_panel->Layout();
}

void epolis::frame::main::on_clean_borders() {
    timer.Stop();
    operation_function.animate_marker_reconstruction(true);
    operation_function.clean_borders(input_image);

    step_images["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_marker_animation_frame()));

    step_images["Step 1 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_input_image_binary())); // binaryzacja
    app_panel->Layout();
    timer.Start(50, wxTIMER_CONTINUOUS);
}

void epolis::frame::main::animate_marker_reconstruction(wxTimerEvent &event) {
    if(operation_function.animate_marker_reconstruction()) {
        image_output->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_clean_borders_result()));
        timer.Stop();
    }

    step_images["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_animation_frame()));
    app_panel->Layout();
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

cv::Mat epolis::frame::main::bitmap_to_mat(const wxStaticBitmap* image) {
    const wxBitmap bitmap = image->GetBitmap();
    const wxImage wx_image = bitmap.ConvertToImage();
    cv::Mat mat(wx_image.GetHeight(), wx_image.GetWidth(), CV_8UC3, wx_image.GetData());
    return mat;
}

cv::Mat epolis::frame::main::bitmap_to_mat_grayscale(const wxStaticBitmap* image) {
    const wxBitmap bitmap = image->GetBitmap();
    const wxImage wx_image = bitmap.ConvertToImage();
    const cv::Mat mat(wx_image.GetHeight(), wx_image.GetWidth(), CV_8UC3, wx_image.GetData());
    cv::Mat gray;
    cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
    return gray;
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
