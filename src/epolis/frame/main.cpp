#include "epolis/frame/main.hpp"
#include "epolis/text/text.hpp"

epolis::frame::main::main(): wxFrame(nullptr, wxID_ANY, "EPOLIS") {
    auto* languageMenu = new wxMenu();
    languageMenu->Append(static_cast<int>(text::lang::en), text::text::translate("English"));
    languageMenu->Append(static_cast<int>(text::lang::pl), text::text::translate("Polish"));

    Bind(wxEVT_MENU, &main::on_change_language, this, static_cast<int>(text::lang::en));
    Bind(wxEVT_MENU, &main::on_change_language, this, static_cast<int>(text::lang::pl));

    auto* menuBar = new wxMenuBar();
    menuBar->Append(languageMenu, text::text::translate("Language"));

    wxFrameBase::SetMenuBar(menuBar);
}

void epolis::frame::main::on_change_language(const wxCommandEvent& event) {
    const auto lang = static_cast<text::lang>(event.GetId());
    text::text::set_language(lang);

    Layout();
    Refresh();
}
