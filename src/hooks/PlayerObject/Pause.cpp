#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(RandomPausePlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = HorribleIdeas::get("pauses");
        int chance = HorribleIdeas::getChance("pauses");
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                auto rnd = Rand::tiny();

                // if the rng is lower than the chance, pause the game
                if (rnd <= m_fields->chance) {
                    log::debug("Pausing the game randomly ({}/5000 < {})", rnd, m_fields->chance);
                    playLayer->pauseGame(true); // pause the game randomly
                };
            };
        };

        PlayerObject::update(p0);
    };
};