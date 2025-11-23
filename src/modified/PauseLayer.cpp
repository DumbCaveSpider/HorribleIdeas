#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        if (auto rightMenu = getChildByID("right-button-menu")) menu::addButton(
            rightMenu,
            this,
            menu_selector(HorriblePauseLayer::onHorribleButton),
            0.6f
        );
    };

    void onHorribleButton(CCObject*) {
        menu::open();
    };
};