#include <filesystem>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "epolis/frame/main.hpp"
#include "epolis/text/text.hpp"

epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS", wxDefaultPosition, wxSize(1280, 720)) {
    auto* outer_sizer = new wxBoxSizer(wxVERTICAL);

    auto* top_menu_sizer = new wxBoxSizer(wxHORIZONTAL);

    const wxArrayString languages = {
        "English",
        "Polish",
    };

    auto* language_choice = new wxChoice(this, static_cast<int>(menu_item::language), wxDefaultPosition, wxDefaultSize, languages, 0);
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
    Bind(wxEVT_BUTTON, &main::on_dilatation, this, static_cast<int>(menu_item::dilatation));
    auto* opening_button = new wxButton(this, static_cast<int>(menu_item::opening), "Opening");
    add_button(opening_button);
    Bind(wxEVT_BUTTON, &main::on_opening, this, static_cast<int>(menu_item::opening));
    auto* closing_button = new wxButton(this, static_cast<int>(menu_item::closing), "Closing");
    add_button(closing_button);
    Bind(wxEVT_BUTTON, &main::on_closing, this, static_cast<int>(menu_item::closing));

    operations_sizer->Add(erosion_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(dilatation_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(opening_button, 0, wxALL|wxEXPAND, 5);
    operations_sizer->Add(closing_button, 0, wxALL|wxEXPAND, 5);

    auto* output_sizer = new wxBoxSizer(wxVERTICAL);

    auto* image_output = new wxStaticBitmap(this, wxID_ANY, get_empty_bitmap());

    output_sizer->Add(image_output, 1, wxEXPAND, 5);

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

void epolis::frame::main::on_change_language(const wxCommandEvent& event) {
    const auto lang = static_cast<text::lang>(event.GetSelection());
    text::text::set_language(lang);
    refresh_text();
}

void epolis::frame::main::on_load_image(const wxCommandEvent& event) {
    const auto item = static_cast<menu_item>(event.GetId());

    auto* dialog = new wxFileDialog(
        this,
        "Select an image",
        (std::filesystem::current_path() / "input").string(),
        "",
        "Supported formats (*.png)|*.png",
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
}

void epolis::frame::main::on_dilatation(const wxCommandEvent& event) {
}

void epolis::frame::main::on_opening(const wxCommandEvent& event) {
}

void epolis::frame::main::on_closing(const wxCommandEvent& event) {
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


wxBitmap epolis::frame::main::get_empty_bitmap() {
    auto bitmap = wxBitmap(256, 256);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    return bitmap;
}
