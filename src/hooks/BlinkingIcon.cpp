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
                if (rnd <= static_cast<int>(horribleMod->getSettingValue<int64_t>("blinking_icon-chance"))) {
                    if (isVisible()) {
                        setVisible(false);
                    } else {
                        setVisible(true);
                    };

                    // log::debug("Toggled player visibility");
                };
            };
        };

        PlayerObject::update(p0);
    };
};