#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "popup/HorribleMenuPopup.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (auto bottomMenu = this->getChildByID("bottom-menu")) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_moonsIcon_001.png",
                0.875f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(MyMenuLayer::onHorribleButton)
            );
            btn->setID("horribleBtn");

            if (auto menu = typeinfo_cast<CCMenu*>(bottomMenu)) {
                menu->addChild(btn);
                menu->updateLayout(true);
            };
        };

        return true;
    };

    void onHorribleButton(CCObject*) {
        if (auto popup = HorribleMenuPopup::create()) popup->show();
    };
};