#include <Horrible.hpp>

#include <menu/FloatingButton.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePlayLayer, PlayLayer) {
    struct Fields {
        bool buttonLevel = horribleMod->getSettingValue<bool>("floating-button-level");
        int64_t buttonOpacity = horribleMod->getSettingValue<int64_t>("floating-button-opacity");

        bool safeMode = horribleMod->getSettingValue<bool>("safe-mode");
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        if (auto fb = FloatingButton::get()) fb->setOpacity(m_fields->buttonLevel ? m_fields->buttonOpacity : 0);
    };

    // safe mode prevents level completion
    void levelComplete() {
        if (m_fields->safeMode) {
            bool testMode = m_isTestMode;

            m_isTestMode = true;
            PlayLayer::levelComplete();
            m_isTestMode = testMode;
        } else {
            log::warn("Safe mode is disabled");
            PlayLayer::levelComplete();
        };
    };
};