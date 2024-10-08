#include <filesystem>

#include "epolis/frame/main.hpp"
#include "epolis/text/text.hpp"

epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS", wxDefaultPosition, wxSize(1280, 720)) {
    auto* outer_sizer = new wxBoxSizer(wxVERTICAL);

    auto* top_menu_sizer = new wxBoxSizer(wxHORIZONTAL);

    const wxArrayString languages = {
        "English",
        "Polish",
    };

    const wxArrayString morph_shapes = {
        "Rectangle",
        "Cross",
        "Ellipse",
    };

    auto* language_choice = new wxChoice(this, static_cast<int>(menu_item::language), wxDefaultPosition, wxDefaultSize, languages, 0);
    add_choice(language_choice,languages);
    Bind(wxEVT_CHOICE, &main::on_change_language, this, static_cast<int>(menu_item::language));
    language_choice->SetSelection(0);

    auto* load_image_1_button = new wxButton(this, static_cast<int>(menu_item::load_image_1), "Load Image 1");
    add_button(load_image_1_button);
    Bind(wxEVT_BUTTON, &main::on_load_image, this, static_cast<int>(menu_item::load_image_1));
    auto* load_image_2_button = new wxButton(this, static_cast<int>(menu_item::load_image_2), "Load Image 2");
    add_button(load_image_2_button);
    Bind(wxEVT_BUTTON, &main::on_load_image, this, static_cast<int>(menu_item::load_image_2));

    top_menu_sizer->Add(language_choice, 0, wxALL, 5);
    top_menu_sizer->Add(load_image_1_button, 0, wxALL, 5);
    top_menu_sizer->Add(load_image_2_button, 0, wxALL, 5);

    auto* main_sizer = new wxBoxSizer(wxHORIZONTAL);

    auto* input_sizer = new wxBoxSizer(wxVERTICAL);

    image_input_1 = new wxStaticBitmap(this, static_cast<int>(image_input::image_1), get_empty_bitmap());
    image_input_1->Bind(wxEVT_LEFT_UP, &main::on_select_image, this, static_cast<int>(image_input::image_1));
    input_sizer->Add(image_input_1, 1, wxEXPAND, 5);

    image_input_2 = new wxStaticBitmap(this, static_cast<int>(image_input::image_2), get_empty_bitmap());
    image_input_2->Bind(wxEVT_LEFT_UP, &main::on_select_image, this, static_cast<int>(image_input::image_2));
    input_sizer->Add(image_input_2, 1, wxEXPAND, 5);

    auto* operations_sizer = new wxBoxSizer(wxVERTICAL);

    auto* erosion_button = new wxButton(this, static_cast<int>(menu_item::erosion), "Erosion");
    add_button(erosion_button);
    Bind(wxEVT_BUTTON, &main::on_erosion, this, static_cast<int>(menu_item::erosion));
    auto* dilatation_button = new wxButton(this, static_cast<int>(menu_item::dilatation), "Dilatation");
    add_button(dilatation_button);
    Bind(wxEVT_BUTTON, &main::on_dilation, this, static_cast<int>(menu_item::dilatation));
    auto* opening_button = new wxButton(this, static_cast<int>(menu_item::opening), "Opening");
    add_button(opening_button);
    Bind(wxEVT_BUTTON, &main::on_opening, this, static_cast<int>(menu_item::opening));
    auto* closing_button = new wxButton(this, static_cast<int>(menu_item::closing), "Closing");
    add_button(closing_button);
    Bind(wxEVT_BUTTON, &main::on_closing, this, static_cast<int>(menu_item::closing));
    auto* fill_holes_button = new wxButton(this, static_cast<int>(menu_item::fill_holes), "Fill Holes");
    add_button(fill_holes_button);
    Bind(wxEVT_BUTTON, &main::on_fill_holes, this, static_cast<int>(menu_item::fill_holes));


    auto* kernel_slider_vSizer = new wxBoxSizer(wxVERTICAL);
    auto* kernel_slider_hSizer = new wxBoxSizer(wxHORIZONTAL);

    constexpr int kernel_slider_starting_value = 1;
    constexpr int kernel_slider_minimum_value = 1;
    constexpr int kernel_slider_maximum_value = 8;

    auto* kernel_slider_title = new wxStaticText(this, wxID_ANY, "Kernel Size");
    add_static_text(kernel_slider_title);
    kernel_slider_vSizer->Add(kernel_slider_title, 0, wxALIGN_CENTER);

    auto* kernel_slider_min_value = new wxStaticText(this, wxID_ANY, std::to_string(kernel_slider_minimum_value));
    kernel_slider_hSizer->Add(kernel_slider_min_value, 0, wxALIGN_LEFT);

    auto* kernel_size_slider = new wxSlider(this, static_cast<int>(menu_item::kernel_size_slider),
        kernel_slider_starting_value, kernel_slider_minimum_value, kernel_slider_maximum_value);
    kernel_slider_hSizer->Add(kernel_size_slider, 1, wxEXPAND | wxLEFT | wxRIGHT, 2);


    auto* kernel_slider_max_value = new wxStaticText(this, wxID_ANY, std::to_string(kernel_slider_maximum_value));
    kernel_slider_hSizer->Add(kernel_slider_max_value, 0, wxALIGN_LEFT);

    kernel_slider_vSizer->Add(kernel_slider_hSizer, 0, wxALL|wxEXPAND, 0);

    auto* kernel_slider_current_value = new wxStaticText(this, wxID_ANY, std::to_string(kernel_size_slider->GetValue()));
    kernel_slider_vSizer->Add(kernel_slider_current_value, 0, wxALIGN_CENTER| wxTOP, 0);

    kernel_size_slider->Bind(wxEVT_SLIDER, [kernel_slider_current_value, this](const wxCommandEvent& event) {
        const int value = event.GetInt();
        kernel_slider_current_value->SetLabel(wxString::Format("%d", value));
        kernel_size_value = value;
    });


    auto* morph_shape_title = new wxStaticText(this, wxID_ANY, "Morph shape");
    add_static_text(morph_shape_title);

    auto* morph_shape_choice = new wxChoice(this, static_cast<int>(menu_item::morph_shape), wxDefaultPosition, wxDefaultSize, morph_shapes, 0);
    add_choice(morph_shape_choice,morph_shapes);
    Bind(wxEVT_CHOICE, &main::on_shape_change, this, static_cast<int>(menu_item::morph_shape));
    morph_shape_choice->SetSelection(0);

    operations_sizer->Add(erosion_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(dilatation_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(opening_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(closing_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(fill_holes_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(kernel_slider_vSizer, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(morph_shape_title, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(morph_shape_choice, 0, wxALL|wxEXPAND, 5);



    auto* output_sizer = new wxBoxSizer(wxVERTICAL);

    auto* save_right_image_button = new wxButton(this, static_cast<int>(menu_item::save_right_image_button), "Save right image");
    add_button(save_right_image_button);
    Bind(wxEVT_BUTTON, &main::on_save_right_image_button, this, static_cast<int>(menu_item::save_right_image_button));

    auto* copy_right_image_to_left_top_button = new wxButton(this, static_cast<int>(menu_item::copy_right_image_to_left_top_button), "Copy to left top image");
    add_button(copy_right_image_to_left_top_button);
    Bind(wxEVT_BUTTON, &main::on_copy_right_image_to_left_top_button, this, static_cast<int>(menu_item::copy_right_image_to_left_top_button));

    auto* copy_right_image_to_left_bottom_button = new wxButton(this, static_cast<int>(menu_item::copy_right_image_to_left_bottom_button), "Copy to left bottom image");
    add_button(copy_right_image_to_left_bottom_button);
    Bind(wxEVT_BUTTON, &main::on_copy_right_image_to_left_bottom_button, this, static_cast<int>(menu_item::copy_right_image_to_left_bottom_button));

    image_output = new wxStaticBitmap(this, wxID_ANY, get_empty_bitmap());

    output_sizer->Add(image_output, 1, wxEXPAND, 5);
    output_sizer->Add(save_right_image_button, 0, wxALIGN_CENTER | wxALL, 5);
    output_sizer->Add(copy_right_image_to_left_top_button, 0, wxALIGN_CENTER | wxALL, 5);
    output_sizer->Add(copy_right_image_to_left_bottom_button, 0, wxALIGN_CENTER | wxALL, 5);

    main_sizer->Add(input_sizer, 1, wxEXPAND, 5);
    main_sizer->Add(operations_sizer, 0, wxEXPAND, 5);
    main_sizer->Add(output_sizer, 1, wxEXPAND, 5);

    outer_sizer->Add(top_menu_sizer, 0, wxEXPAND, 5);
    outer_sizer->Add(main_sizer, 1, wxEXPAND, 5);

    SetSizer(outer_sizer);
    wxTopLevelWindowBase::Layout();
    Centre(wxBOTH);

    select_image(image_input::image_1);
    refresh_text();
}

void epolis::frame::main::on_shape_change(const wxCommandEvent& event) {
    switch (event.GetSelection()) {
        case 0:
            morph_shape = cv::MORPH_RECT;
        break;
        case 1:
            morph_shape = cv::MORPH_CROSS;
        break;
        case 2:
            morph_shape = cv::MORPH_ELLIPSE;
        break;
        default:
            wxLogWarning("Unknown selection.");
        break;
    }
}

void epolis::frame::main::on_change_language(const wxCommandEvent& event) {
    const auto lang = static_cast<text::lang>(event.GetSelection());
    text::text::set_language(lang);
    refresh_text();
    Layout();
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
    const auto image = cv::imread(std::string(path), cv::IMREAD_COLOR);

    const wxImage wx_image(image.cols, image.rows, image.data, true);

    if(item == menu_item::load_image_1) {
        image_input_1->SetBitmap(wxBitmap(wx_image));
    }
    else if(item == menu_item::load_image_2) {
        image_input_2->SetBitmap(wxBitmap(wx_image));
    }

    Layout();
}

void epolis::frame::main::on_select_image(const wxMouseEvent& event) {
    const auto selected_image = static_cast<image_input>(event.GetId());

    select_image(selected_image);
}

void epolis::frame::main::on_erosion(const wxCommandEvent& event) {
    const auto erosion_type = morph_shape;
    const auto erosion_size = kernel_size_value;

    const cv::Mat source = bitmap_to_mat(selected_input);
    cv::Mat destination;

    const cv::Mat element = getStructuringElement(
        erosion_type,
        cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
        cv::Point(erosion_size, erosion_size)
    );

    erode(source, destination, element);

    image_output->SetBitmap(mat_to_bitmap(destination));
    Layout();
}

void epolis::frame::main::on_dilation(const wxCommandEvent& event) {
    const auto dilation_type = morph_shape;
    const auto dilation_size = kernel_size_value;

    const cv::Mat source = bitmap_to_mat(selected_input);
    cv::Mat destination;

    const cv::Mat element = getStructuringElement(
        dilation_type,
        cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
        cv::Point(dilation_size, dilation_size)
    );

    dilate(source, destination, element);

    image_output->SetBitmap(mat_to_bitmap(destination));
    Layout();
}

void epolis::frame::main::on_opening(const wxCommandEvent& event) {
    const auto morph_element = morph_shape;
    constexpr auto morph_operation = cv::MORPH_OPEN;
    const auto morph_size = kernel_size_value;

    const cv::Mat source = bitmap_to_mat(selected_input);
    cv::Mat destination;

    const cv::Mat element = getStructuringElement(
        morph_element,
        cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
        cv::Point(morph_size, morph_size)
    );

    morphologyEx(source, destination, morph_operation, element);

    image_output->SetBitmap(mat_to_bitmap(destination));
    Layout();
}

void epolis::frame::main::on_closing(const wxCommandEvent& event) {
    const auto morph_element = morph_shape;
    constexpr auto morph_operation = cv::MORPH_CLOSE;
    const auto morph_size = kernel_size_value;

    const cv::Mat source = bitmap_to_mat(selected_input);
    cv::Mat destination;

    const cv::Mat element = getStructuringElement(
        morph_element,
        cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
        cv::Point(morph_size, morph_size)
    );

    morphologyEx(source, destination, morph_operation, element);

    image_output->SetBitmap(mat_to_bitmap(destination));
    Layout();
}
void epolis::frame::main::on_fill_holes(const wxCommandEvent &event) {
    const cv::Mat source = bitmap_to_mat(selected_input);
    cv::Mat destination,threshold,flood_fill,inv,gray;

    cv::cvtColor(source, gray, cv::COLOR_BGR2GRAY);

    cv::threshold(gray,threshold, 220, 255, cv::THRESH_BINARY);
    cv::Mat mask = cv::Mat::zeros(threshold.rows + 2, threshold.cols + 2, CV_8UC1);
    flood_fill = threshold.clone();
     cv::floodFill(flood_fill,mask, cv::Point(0,0), cv::Scalar(255));

    cv::bitwise_not(flood_fill, inv);

    destination = (threshold | inv);

    image_output->SetBitmap(mat_to_bitmap_greyscale(destination));
    Layout();
}

void epolis::frame::main::on_save_right_image_button(const wxCommandEvent& event) {
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

void epolis::frame::main::on_copy_right_image_to_left_top_button(const wxCommandEvent& event) {
    const wxBitmap outputBitmap = image_output->GetBitmap();
    image_input_1->SetBitmap(outputBitmap);
    Layout();
}
void epolis::frame::main::on_copy_right_image_to_left_bottom_button(const wxCommandEvent& event) {
    const wxBitmap outputBitmap = image_output->GetBitmap();
    image_input_2->SetBitmap(outputBitmap);
    Layout();
}

void epolis::frame::main::select_image(image_input image) {
    const static auto color = wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION);

    if(image == image_input::image_1) {
        selected_input = image_input_1;
        image_input_1->SetBackgroundColour(color);
        image_input_2->SetBackgroundColour(wxNullColour);
    }
    else if(image == image_input::image_2) {
        selected_input = image_input_2;
        image_input_2->SetBackgroundColour(color);
        image_input_1->SetBackgroundColour(wxNullColour);
    }

    Layout();
}

cv::Mat epolis::frame::main::bitmap_to_mat(const wxStaticBitmap* image) {
    const wxBitmap bitmap = image->GetBitmap();
    const wxImage wx_image = bitmap.ConvertToImage();
    cv::Mat mat(wx_image.GetHeight(), wx_image.GetWidth(), CV_8UC3, wx_image.GetData());
    return mat;
}

wxBitmap epolis::frame::main::mat_to_bitmap(const cv::Mat& image) {
    const wxImage wx_image(image.cols, image.rows, image.data, true);
    auto bitmap = wxBitmap(wx_image);
    return bitmap;
}

wxBitmap epolis::frame::main::mat_to_bitmap_greyscale(const cv::Mat& image) {
    cv::Mat grayscale_rgb;

    cvtColor(image, grayscale_rgb, cv::COLOR_GRAY2BGR);
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
