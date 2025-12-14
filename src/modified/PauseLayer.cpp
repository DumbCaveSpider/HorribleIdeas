#include <Utils.hpp>

#include <menu/FloatingButton.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        if (auto fb = FloatingButton::get()) fb->setVisible(horribleMod->getSettingValue<bool>("floating-button"));
    };
};