#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(BlinkingIconPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("blinking_icon");
        int chance = options::getChance("blinking_icon");

        float m_delay = 0.f;
    };

    void update(float dt) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                int rnd = randng::tiny();

                // randomly makes the player invisible and visible again
                if (rnd <= m_fields->chance) {
                    m_fields->m_delay += dt;

                    auto r = static_cast<float>(rnd);
                    if (m_fields->m_delay >= (isVisible() ? 0.875f * rnd : 0.125f * rnd)) setVisible(!isVisible());
                };
            };
        };

        PlayerObject::update(dt);
    };
};