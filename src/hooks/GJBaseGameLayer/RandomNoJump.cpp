#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(NoJumpGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = horribleideas::get("no_jump");
        float chance = horribleideas::get("no_jump");
    };

    void handleButton(bool down, int button, bool isPlayer1) {
        if (m_fields->enabled) {
            if (button == 1) {
                auto rng = randng::fast();
                // log::debug("button chance {}", rng);

                if (rng <= m_fields->chance) {
                    log::error("skipped dat jump");
                    GJBaseGameLayer::handleButton(false, button, isPlayer1);

                    return;
                };
            };
        };

        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    };
};