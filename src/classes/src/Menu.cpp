#include "../Menu.hpp"

#include <menu/HorribleMenuPopup.hpp>

#include <Horrible.hpp>

using namespace horrible;

void menu::open() {
    static Ref<HorribleMenuPopup> menu = nullptr;
    if (menu) {
        menu->removeMeAndCleanup();
        menu = nullptr;
    } else if (auto popup = HorribleMenuPopup::create()) {
        popup->show();
        menu = popup;
    };
};