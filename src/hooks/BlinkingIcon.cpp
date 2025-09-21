#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(BlinkingIconPlayerObject, PlayerObject) {
    struct Fields {
        int chance = 0;
    };

    bool init(int player, int ship, GJBaseGameLayer * gameLayer, CCLayer * layer, bool playLayer) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) return false;

        m_fields->chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("blinking_icon-chance"));
        log::debug("Blinking icon chance set to {}", m_fields->chance);

        return true;
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (horribleMod->getSavedValue<bool>("blinking_icon", false)) {
                auto rnd = Rand::fast();

                // randomly makes the player invisible and visible again
                if (rnd <= m_fields->chance) {
                    if (isVisible()) {
                        setVisible(false);
                    } else {
                        setVisible(true);
                    };

                    // log::debug("Toggled player visibility");
                };
            };
        };

        PlayerObject::update(p0);
    };
};