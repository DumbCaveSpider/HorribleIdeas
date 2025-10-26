#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(BlinkingIconPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = horribleideas::get("blinking_icon");
        int chance = horribleideas::getChance("blinking_icon");
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                auto rnd = Rand::tiny();

                // randomly makes the player invisible and visible again
                if (rnd <= m_fields->chance) {
                    setVisible(!isVisible());

                    // log::debug("Toggled player visibility");
                };
            };
        };

        PlayerObject::update(p0);
    };
};