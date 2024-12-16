#include <filesystem>
#include <wx/wrapsizer.h>

#include "epolis/frame/main.hpp"
#include "epolis/text/text.hpp"
#include "epolis/style.hpp"

#include "epolis/frame/custom/triangle_slider.hpp"


epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS", wxDefaultPosition, wxSize(1280, 720)) {
    app_panel = new wxPanel(this, wxID_ANY);

    app_panel->SetFont(app_panel->GetFont().Scale(SCALE));

    app_panel->SetDoubleBuffered(true);

    layout.initialise_layout(app_panel);
    layout.initialise_operation_bitmaps(app_panel);


    Bind(wxEVT_BUTTON, &main::on_change_language, this, static_cast<int>(menu_item::language));
    Bind(wxEVT_CHOICE, &main::on_change_operation, this, static_cast<int>(menu_item::operations));
    Bind(wxEVT_BUTTON, &main::on_load_image, this, static_cast<int>(menu_item::load_image_1));
    Bind(wxEVT_BUTTON, &main::on_run_button, this, static_cast<int>(menu_item::run_button));
    layout.get_timer_slider()->Bind(wxEVT_TRIANGULAR_SLIDER_CHANGED, &main::on_timer_slider, this);
    Bind(wxEVT_BUTTON, &main::on_save_image_button, this, static_cast<int>(menu_item::save_right_image_button));
    Bind(wxEVT_TIMER, &main::animate_marker_reconstruction, this);
    Bind(wxEVT_BUTTON, &main::on_previous_step, this, static_cast<int>(menu_item::previous_step));
    Bind(wxEVT_BUTTON, &main::on_animation_pause, this, static_cast<int>(menu_item::stop));
    Bind(wxEVT_BUTTON, &main::on_animation_resume, this, static_cast<int>(menu_item::start));
    Bind(wxEVT_BUTTON, &main::on_next_step, this, static_cast<int>(menu_item::next_step));

    timer.SetOwner(this);

    Centre(wxBOTH);

    wxTopLevelWindow::Maximize(true);

    on_change_operation(layout.get_initial_selection());
}

void epolis::frame::main::on_run_button(const wxCommandEvent& event) {
    if (input_image.empty()) {
        return;
    }

    app_panel->SetDoubleBuffered(false);

    wxString operation = layout.on_run_button();

    if (operation == "Clean borders") {
        on_clean_borders();
    } else if (operation == "Fill holes") {
        on_fill_holes();
    }
    pause = false;
    is_running = true;
    operation_function.clear_buffer();

    app_panel->Layout();
}

void epolis::frame::main::on_timer_slider(const wxCommandEvent& event) {
    if(timer.IsRunning()) {
        std::cout << layout.get_timer_slider()->GetValue() << std::endl;
        timer.Stop();
        timer.Start(layout.get_timer_slider()->GetValue(), wxTIMER_CONTINUOUS);
    }
}

void epolis::frame::main::on_previous_step(const wxCommandEvent &event) {
    if(pause && is_running) {
        layout.get_step_images()["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_previous_image()));
    }
    else {
        wxCommandEvent dummyEvent(wxEVT_BUTTON, static_cast<int>(menu_item::stop));
        on_animation_pause(dummyEvent);
    }
}

void epolis::frame::main::on_animation_pause(const wxCommandEvent &event) {
    if(is_running) {
        layout.on_animation_pause();
        timer.Stop();
        pause = true;

        app_panel->Layout();
    }
}

void epolis::frame::main::on_animation_resume(const wxCommandEvent &event) {
    if(is_running) {
        layout.on_animation_resume();
        timer.Start(layout.get_timer_slider()->GetValue(), wxTIMER_CONTINUOUS);
        pause = false;

        app_panel->Layout();
    }
}

void epolis::frame::main::on_next_step(const wxCommandEvent &event) {
    if(pause && is_running) {
        if(operation_function.animate_marker_reconstruction()) {
            layout.get_image_output()->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_destination()));
        }
        layout.get_step_images()["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_animation_frame()));
    }
    else {
        wxCommandEvent dummyEvent(wxEVT_BUTTON, static_cast<int>(menu_item::stop));
        on_animation_pause(dummyEvent);
    }
}


void epolis::frame::main::on_change_language(const wxCommandEvent& event) {
    text::text::set_language(layout.on_change_language());
    layout.refresh_text();
    app_panel->Layout();
}

void epolis::frame::main::on_change_operation(const wxCommandEvent& event) {
    timer.Stop();
    operation_function.animate_marker_reconstruction(true);

    layout.on_change_operation(event);

    pause = false;
    is_running = false;
    operation_function.clear_buffer();

    layout.refresh_text();
    app_panel->Layout();
}

void epolis::frame::main::on_load_image(const wxCommandEvent& event) {
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

    is_running = false;
    operation_function.clear_buffer();

    if (input_image.cols > IMAGE_WIDTH || input_image.rows > IMAGE_HEIGHT
        || input_image.cols < IMAGE_WIDTH || input_image.rows < IMAGE_HEIGHT) {
        cv::resize(input_image, input_image, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));  // resize to match bitmap size
    }

    // Convert BGR to RGB
    cv::Mat rgb_image;
    cv::cvtColor(input_image, rgb_image, cv::COLOR_BGR2RGB);

    const wxImage wx_image(rgb_image.cols, rgb_image.rows, rgb_image.data, true);
    layout.on_load_image(wx_image);

    app_panel->Layout();
}

void epolis::frame::main::on_fill_holes() {
    operation_function.animate_marker_reconstruction(true);
    operation_function.fill_holes(input_image);

    layout.get_step_images()["Step 1 Fill"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_input_image_binary())); //binaryzacja
    layout.get_step_images()["Step 2 Fill"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_inverted_image())); //negacja
    layout.get_step_images()["Step 3 Fill"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_destination())); //markery
    layout.get_image_output()->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_result())); //wynik koncowy

    app_panel->SetDoubleBuffered(true);
    app_panel->Layout();
}

void epolis::frame::main::on_clean_borders() {
    timer.Stop();
    operation_function.animate_marker_reconstruction(true);
    operation_function.clean_borders(input_image);

    layout.get_step_images()["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_marker_animation_frame()));

    layout.get_step_images()["Step 1 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_input_image_binary())); // binaryzacja
    app_panel->Layout();
    timer.Start(layout.get_timer_slider()->GetValue(), wxTIMER_CONTINUOUS);
}

void epolis::frame::main::animate_marker_reconstruction(wxTimerEvent &event) {
    if(operation_function.animate_marker_reconstruction()) {
        layout.get_image_output()->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_destination()));
        timer.Stop();
        app_panel->SetDoubleBuffered(true);
    }

    layout.get_step_images()["Step 2 Clean"]->SetBitmap(mat_to_bitmap_greyscale(operation_function.get_animation_frame()));
    // app_panel->Layout();
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

    const wxBitmap bitmap = layout.get_image_output()->GetBitmap();

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

