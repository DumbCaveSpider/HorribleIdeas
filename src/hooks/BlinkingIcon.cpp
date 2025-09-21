#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(BlinkingIconPlayerObject, PlayerObject) {
    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (horribleMod->getSavedValue<bool>("blinking_icon", false)) {
                auto rnd = rand() % 101;
                // randomly makes the player invisible and visible again
                if (rnd <= horribleMod->getSettingValue<int>("blinking_icon-chance")) {
                    if (this->isVisible()) {
                        this->setVisible(false);
                    } else {
                        this->setVisible(true);
                    }
                    // log::debug("Toggled player visibility");
                }
            }
        }
        PlayerObject::update(p0);
    };
};