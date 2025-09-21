#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

static RandomSeeder _randomSeeder;

class $modify(RandomSpeedPlayerObject, PlayerObject) {
    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (horribleMod->getSavedValue<bool>("random_speed", false)) {
                auto rnd = rand() % 101;
                // if the rng is lower than the chance, change the speed
                if (rnd <= horribleMod->getSettingValue<int>("speed-change-chance")) {
                    // randomly choose a new speed between 10% and 200%
                    auto newSpeed = (rand() % 191 + 10) / 100.0f;
                    this->m_playerSpeed = static_cast<float>(newSpeed);
                    log::debug("Changed player speed to {}", newSpeed);
                }
            }
        }

        PlayerObject::update(p0);
    };
};
