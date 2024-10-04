#include <filesystem>

#include "epolis/epolis.hpp"
#include "epolis/frame/main.hpp"
#include "epolis/text/text.hpp"

bool epolis::epolis::OnInit() {
    wxString error;

    try {
        if(!std::filesystem::exists("output")) {
            std::filesystem::create_directory("output");
        }

        text::text::load_text();

        auto* frame = new frame::main();
        frame->Show();
        return true;
    }
    catch(const std::exception& e) {
        error = e.what();
    }
    catch(...) {
        error = "unknown error.";
    }

    wxMessageBox(error, "Error", wxICON_ERROR | wxOK);
    return false;
}

