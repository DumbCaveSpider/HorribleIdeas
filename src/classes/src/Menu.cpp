#include "../Menu.hpp"

#include <menu/HorribleMenuPopup.hpp>

#include <Horrible.hpp>

using namespace horrible;

void menu::open() {
    if (auto existing = CCScene::get()->getChildByID("options"_spr)) {
        existing->removeFromParentAndCleanup(true);
        return;
    };

    if (auto popup = HorribleMenuPopup::create()) popup->show();
};