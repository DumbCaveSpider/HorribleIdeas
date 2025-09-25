#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(NoJumpGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("no_jump", false);
        float chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("no_jump-chance"));
    };

    void handleButton(bool down, int button, bool isPlayer1) {
        if (m_fields->enabled) {
            if (button == 1) {
                auto rng = Rand::fast();
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