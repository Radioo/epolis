#include <filesystem>

#include "epolis/frame/main.hpp"
#include "epolis/text/text.hpp"

epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS") {
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

    outer_sizer->Add(top_menu_sizer, 1, wxEXPAND, 5);

    SetSizer(outer_sizer);
    wxTopLevelWindowBase::Layout();
    Centre(wxBOTH);

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
        std::filesystem::current_path().string(),
        "",
        "Supported formats (*.png)|*.png",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if(dialog->ShowModal() == wxID_CANCEL) {
        return;
    }
}
