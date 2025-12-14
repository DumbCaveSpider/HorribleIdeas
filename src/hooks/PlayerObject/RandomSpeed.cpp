#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(RandomSpeedPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("random_speed");
        int chance = options::getChance("random_speed");
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                int rnd = randng::tiny();

                // if the rng is lower than the chance, change the speed
                if (rnd <= m_fields->chance) {
                    // randomly choose a new speed between 10% and 200%
                    auto newSpeed = (rand() % 191 + 10) / 100.f;
                    m_playerSpeed = static_cast<float>(newSpeed);

                    log::debug("Changed player speed to {}", newSpeed);
                };
            };
        };

        PlayerObject::update(p0);
    };
};
