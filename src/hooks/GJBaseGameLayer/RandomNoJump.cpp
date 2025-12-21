#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(NoJumpGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = options::get("no_jump");
        int chance = options::get("no_jump");
    };

    void handleButton(bool down, int button, bool isPlayer1) {
        if (m_fields->enabled) {
            if (button == 1) {
                int rnd = randng::fast();
                // log::debug("button chance {}", rng);

                if (rnd <= m_fields->chance) {
                    log::error("skipped dat jump");
                    GJBaseGameLayer::handleButton(false, button, isPlayer1);

                    return;
                };
            };
        };

        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    };
};