#include "../Menu.hpp"

#include <menu/HorribleMenuPopup.hpp>

#include <Horrible.hpp>

using namespace horrible;

void menu::open() {
    if (auto popup = HorribleMenuPopup::get()) {
        popup->removeMeAndCleanup();
    } else if (auto popup = HorribleMenuPopup::create()) {
        popup->show();
    };
};