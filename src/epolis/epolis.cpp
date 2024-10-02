#include "epolis/epolis.hpp"
#include "epolis/frame/main.hpp"

bool epolis::epolis::OnInit() {
    auto* frame = new frame::main();
    frame->Show();
    return true;
}
