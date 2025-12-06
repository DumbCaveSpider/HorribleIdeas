#include <Horrible.hpp>

#include <menu/FloatingButton.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePlayLayer, PlayLayer) {
    struct Fields {
        bool safeMode = horribleMod->getSettingValue<bool>("safe-mode");
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        hideButton();
    };

    void resume() {
        PlayLayer::resume();

        hideButton();
    };

    void resumeAndRestart(bool fromStart) {
        PlayLayer::resumeAndRestart(fromStart);

        hideButton();
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

    void hideButton() {
        if (auto fb = FloatingButton::get()) fb->setOpacity(fb->showInLevel() ? fb->getOpacitySetting() : 0);
    };
};