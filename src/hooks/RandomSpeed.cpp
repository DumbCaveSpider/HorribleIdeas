#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;



class $modify(RandomSpeedPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("random_speed", false);
        int chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("random_speed-chance"));
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                auto rnd = Rand::fast();

                // if the rng is lower than the chance, change the speed
                if (rnd <= m_fields->chance) {
                    // randomly choose a new speed between 10% and 200%
                    auto newSpeed = (rand() % 191 + 10) / 100.0f;
                    m_playerSpeed = static_cast<float>(newSpeed);

                    log::debug("Changed player speed to {}", newSpeed);
                };
            };
        };

        PlayerObject::update(p0);
    };
};
