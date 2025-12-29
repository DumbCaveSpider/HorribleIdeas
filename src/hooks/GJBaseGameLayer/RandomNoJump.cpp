#include <Utils.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

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
                if (rnd <= m_fields->chance) return GJBaseGameLayer::handleButton(false, button, isPlayer1);
            };
        };

        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    };
};