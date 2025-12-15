#include "../Menu.hpp"

#include <menu/HorribleMenuPopup.hpp>

#include <Utils.hpp>

using namespace horrible;

void menu::open() {
    if (auto old = HorribleMenuPopup::get()) {
        old->removeMeAndCleanup();
    } else if (auto popup = HorribleMenuPopup::create()) {
        popup->show();
    };
};