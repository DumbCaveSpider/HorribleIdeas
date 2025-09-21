#include <Horrible.hpp>

#include <menu/HorribleMenuPopup.hpp>

#include <fmt/core.h>

#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorribleMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto gm = GameManager::get();

        // get and store user current fps
        float currentFPS = gm->m_customFPSTarget;
        float storedFPS = horribleMod->setSavedValue<float>("fps", currentFPS);

        log::debug("Store Current FPS: {}", storedFPS);

        if (auto bottomMenu = getChildByID("bottom-menu")) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_moonsIcon_001.png",
                0.875f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(HorribleMenuLayer::onHorribleButton)
            );
            btn->setID("menu-btn"_spr);

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