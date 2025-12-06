#include <Horrible.hpp>

#include <menu/FloatingButton.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePauseLayer, PauseLayer) {
    struct Fields {
        int64_t buttonOpacity = horribleMod->getSettingValue<int64_t>("floating-button-opacity");
    };

    void customSetup() {
        PauseLayer::customSetup();

        if (auto fb = FloatingButton::get()) fb->setOpacity(fb->getOpacitySetting());
    };
};