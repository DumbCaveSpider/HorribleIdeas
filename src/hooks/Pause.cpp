#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

static RandomSeeder _randomSeeder;

class $modify(RandomPausePlayerObject, PlayerObject) {
    struct Fields {
        int chance = 0;
    };

    bool init(int player, int ship, GJBaseGameLayer * gameLayer, CCLayer * layer, bool playLayer) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) return false;

        m_fields->chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("pauses-chance"));
        log::debug("Random pause chance set to {}", m_fields->chance);

        return true;
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (horribleMod->getSavedValue<bool>("pauses", false)) {
                auto rnd = rand() % 101;

                // if the rng is lower than the chance, pause the game
                if (rnd <= m_fields->chance) {
                    log::debug("Pausing the game randomly");
                    playLayer->pauseGame(true); // pause the game randomly
                };
            };
        };

        PlayerObject::update(p0);
    };
};