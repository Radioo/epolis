#include "epolis/text/dynamic_text.hpp"
#include "epolis/text/text.hpp"

void epolis::text::dynamic_text::refresh_text() {
    for(const auto& [button, text] : buttons) {
        button->SetLabel(wxString::FromUTF8(text::translate(text)));
    }
    for(const auto& [static_text, text] : static_texts) {
        static_text->SetLabel(wxString::FromUTF8(text::translate(text)));
    }
    for(const auto& [choice_Box, array_values] : choices) {
        int n = choice_Box->GetSelection();
        choice_Box->Clear();
        for(int i = 0; i < array_values.size(); i++) {
            choice_Box->Append(wxString::FromUTF8(text::translate(array_values.Item(i).ToStdString())));
        }
        choice_Box->Select(n);
    }
}

void epolis::text::dynamic_text::add_button(wxButton* button) {
    buttons[button] = button->GetLabel();
}

void epolis::text::dynamic_text::add_static_text(wxStaticText* text) {
    static_texts[text] = text->GetLabel();
}

void epolis::text::dynamic_text::add_choice(wxChoice* choice, const wxArrayString& values) {
    choices[choice] = values;
}