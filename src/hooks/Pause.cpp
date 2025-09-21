#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

static RandomSeeder _randomSeeder;

class $modify(RandomPausePlayerObject, PlayerObject) {
    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (horribleMod->getSavedValue<bool>("pauses", false)) {
                auto rnd = rand() % 101;
                // if the rng is lower than the chance, pause the game
                if (rnd <= horribleMod->getSettingValue<int>("pauses-chance")) {
                    log::debug("Pausing the game randomly");
                    playLayer->pauseGame(true); // pause the game randomly
                }
            }
        }

        PlayerObject::update(p0);
    };
};