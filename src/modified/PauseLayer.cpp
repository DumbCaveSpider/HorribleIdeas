#include <Horrible.hpp>

#include <menu/FloatingButton.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePauseLayer, PauseLayer) {
    struct Fields {
        bool floatingBtn = horribleMod->getSettingValue<bool>("floating-button");
    };

    void customSetup() {
        PauseLayer::customSetup();

        if (auto fb = FloatingButton::get()) fb->setVisible(m_fields->floatingBtn);
    };
};