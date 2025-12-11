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

        toggleButton();
    };

    void resume() {
        PlayLayer::resume();

        toggleButton();
    };

    void resumeAndRestart(bool fromStart) {
        PlayLayer::resumeAndRestart(fromStart);

        toggleButton();
    };

    void showEndLayer() {
        PlayLayer::showEndLayer();

        toggleButton(true);
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

    void toggleButton(bool toggle = false) {
        if (auto fb = FloatingButton::get()) fb->setVisible(fb->showInLevel() || toggle);
    };
};