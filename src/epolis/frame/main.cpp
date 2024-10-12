#include <filesystem>

#include "epolis/frame/main.hpp"

#include <wx/wrapsizer.h>

#include "epolis/text/text.hpp"

epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS", wxDefaultPosition, wxSize(1280, 720)), timer(this) {
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

    auto* save_image_button = new wxButton(this, static_cast<int>(menu_item::save_right_image_button), "Save image");
    add_button(save_image_button);
    Bind(wxEVT_BUTTON, &main::on_save_image_button, this, static_cast<int>(menu_item::save_right_image_button));

    top_menu_sizer->Add(language_choice, 0, wxALL, 5);
    top_menu_sizer->Add(load_image_1_button, 0, wxALL, 5);
    top_menu_sizer->AddStretchSpacer(1);
    top_menu_sizer->Add(save_image_button, 0, wxALL, 5);

    outer_sizer->Add(top_menu_sizer, 0, wxEXPAND, 5);

    auto* images_sizer = new wxWrapSizer(wxHORIZONTAL, wxALIGN_CENTER_HORIZONTAL);

    image_input_1 = new wxStaticBitmap(this, wxID_ANY, get_empty_bitmap());
    images_sizer->Add(image_input_1, 1, wxALL | wxEXPAND, 5);

    step_image_1 = new wxStaticBitmap(this, wxID_ANY, get_empty_bitmap());
    images_sizer->Add(step_image_1, 1, wxALL | wxEXPAND, 5);

    step_image_2 = new wxStaticBitmap(this, wxID_ANY, get_empty_bitmap());
    images_sizer->Add(step_image_2, 1, wxALL | wxEXPAND, 5);

    step_image_3 = new wxStaticBitmap(this, wxID_ANY, get_empty_bitmap());
    images_sizer->Add(step_image_3, 1, wxALL | wxEXPAND, 5);

    image_output = new wxStaticBitmap(this, wxID_ANY, get_empty_bitmap());
    images_sizer->Add(image_output, 1, wxALL | wxEXPAND, 5);

    outer_sizer->Add(images_sizer, 1, wxEXPAND, 5);


    SetSizer(outer_sizer);
    wxTopLevelWindowBase::Layout();
    Centre(wxBOTH);
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
    timer.Stop();
    step = 0;
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
    timer.Stop();
    step = 0;
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
    timer.Stop();
    step = 0;
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
    timer.Stop();
    step = 0;
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

    step = 0;

    timer.Start(1000,wxTIMER_ONE_SHOT);
}

void epolis::frame::main::process_on_fill_holes(wxTimerEvent &event) {
     // const cv::Mat source = bitmap_to_mat(selected_input);
     // cv::Mat threshold,flood_fill,inv,inv2,gray;
    //
    // cv::cvtColor(source, gray, cv::COLOR_BGR2GRAY);
    //
    // cv::threshold(gray,threshold, 220, 255, cv::THRESH_OTSU);
    // cv::Mat mask = cv::Mat::zeros(threshold.rows + 2, threshold.cols + 2, CV_8UC1);
    // flood_fill = threshold.clone();
    //
    // cv::floodFill(flood_fill, mask, cv::Point(0, 0), cv::Scalar(255));
    //
    // cv::bitwise_not(flood_fill, inv);
    //
    // destination = (threshold | inv);
    //
    // image_output->SetBitmap(mat_to_bitmap_greyscale(destination));
    //
    // Layout();

    switch (step) {
        case 0: {
            const cv::Mat source = bitmap_to_mat_grayscale(selected_input);
            cv::Mat threshold;
            cv::threshold(source, threshold, 220, 255, cv::THRESH_OTSU);

            image_output->SetBitmap(mat_to_bitmap_greyscale(threshold));
            Layout();

            step++;
            break;
        }

        case 1: {
            cv::Mat threshold = bitmap_to_mat_grayscale(image_output);
            cv::Mat flood_fill;
            cv::Mat mask = cv::Mat::zeros(threshold.rows + 2, threshold.cols + 2, CV_8UC1);
            flood_fill = threshold.clone();
            cv::floodFill(flood_fill, mask, cv::Point(0, 0), cv::Scalar(255));

            image_output->SetBitmap(mat_to_bitmap_greyscale(flood_fill));
            Layout();

            step++;
            break;
        }

        case 2: {
            cv::Mat flood_fill = bitmap_to_mat_grayscale(image_output);
            cv::Mat inv;
            cv::bitwise_not(flood_fill, inv);

            image_output->SetBitmap(mat_to_bitmap_greyscale(inv));
            Layout();

            step++;
            break;
        }

        case 3: {
            cv::Mat inv = bitmap_to_mat_grayscale(image_output);
            cv::Mat destination;
            const cv::Mat source = bitmap_to_mat_grayscale(selected_input);
            cv::Mat threshold;
            cv::threshold(source, threshold, 220, 255, cv::THRESH_OTSU);
            destination = (threshold | inv);

            image_output->SetBitmap(mat_to_bitmap_greyscale(destination));
            Layout();

            timer.Stop();
            step = 0;
            return;
        }
    }
    timer.Start(4000,wxTIMER_ONE_SHOT);
}


void epolis::frame::main::on_clear_borders(const wxCommandEvent &event) {
    timer.Stop();
    step = 0;
    const cv::Mat source = bitmap_to_mat(selected_input);
    cv::Mat threshold, gray;

    cv::cvtColor(source, gray, cv::COLOR_BGR2GRAY);

    cv::threshold(gray, threshold, 220, 255, cv::THRESH_OTSU);

    cv::Mat mask = cv::Mat::zeros(threshold.rows + 2, threshold.cols + 2, CV_8UC1);
    cv::Mat flood_fill = threshold.clone();
    for (int col = 0; col < flood_fill.cols; ++col) {
        cv::floodFill(flood_fill, mask, cv::Point(col, 0), cv::Scalar(0));
        cv::floodFill(flood_fill, mask, cv::Point(col, flood_fill.rows - 1), cv::Scalar(0));
    }
    for (int row = 0; row < flood_fill.rows; ++row) {
        cv::floodFill(flood_fill, mask, cv::Point(0, row), cv::Scalar(0));
        cv::floodFill(flood_fill, mask, cv::Point(flood_fill.cols - 1, row), cv::Scalar(0));
    }

    image_output->SetBitmap(mat_to_bitmap_greyscale(flood_fill));
    Layout();
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

cv::Mat epolis::frame::main::bitmap_to_mat_grayscale(const wxStaticBitmap* image) {
    const wxBitmap bitmap = image->GetBitmap();
    const wxImage wx_image = bitmap.ConvertToImage();
    cv::Mat mat(wx_image.GetHeight(), wx_image.GetWidth(), CV_8UC3, wx_image.GetData()), gray;
    cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
    return gray;
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
