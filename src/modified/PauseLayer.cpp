#include <Horrible.hpp>

#include <menu/HorribleMenuPopup.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        if (auto rightMenu = getChildByID("right-button-menu")) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_moonsIcon_001.png",
                0.875f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );
            btnSprite->setScale(0.6f);

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(HorriblePauseLayer::onHorribleButton)
            );
            btn->setID("menu-btn"_spr);

            if (auto menu = typeinfo_cast<CCMenu*>(rightMenu)) {
                menu->addChild(btn);
                menu->updateLayout(true);
            };
        };
    };

    void onHorribleButton(CCObject*) {
        if (auto popup = HorribleMenuPopup::create()) popup->show();
    };
};