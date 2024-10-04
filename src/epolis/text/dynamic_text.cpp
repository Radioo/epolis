#include "epolis/text/dynamic_text.hpp"
#include "epolis/text/text.hpp"

void epolis::text::dynamic_text::refresh_text() {
    for(const auto& [button, text] : buttons) {
        button->SetLabel(text::text::translate(text));
    }
}

void epolis::text::dynamic_text::add_button(wxButton* button) {
    buttons[button] = button->GetLabel();
}