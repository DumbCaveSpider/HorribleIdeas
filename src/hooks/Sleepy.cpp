#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

static RandomSeeder _randomSeeder;

class $modify(SleepyPlayerObject, PlayerObject) {
    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            // sleepy player
            if (horribleMod->getSavedValue<bool>("sleepy", false)) {
                log::debug("Player is sleepy!");
            };
        };

        PlayerObject::update(p0);
    };
};
