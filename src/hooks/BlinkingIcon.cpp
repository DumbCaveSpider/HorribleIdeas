#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(BlinkingIconPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("blinking_icon", false);
        int chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("blinking_icon-chance"));
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                auto rnd = Rand::fast();

                // randomly makes the player invisible and visible again
                if (rnd <= m_fields->chance) {
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