#include <menu/FloatingButton.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(HorriblePauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        if (auto fb = FloatingButton::get()) fb->setOpacity(fb->getOpacitySetting());
    };
};