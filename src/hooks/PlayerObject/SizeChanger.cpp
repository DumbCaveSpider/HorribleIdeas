#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(SizeChangerPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("size_changer");
        int chance = options::getChance("size_changer");

        bool scaled = false;
    };

    bool pushButton(PlayerButton button) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                // log::debug("size changer jump detected");
                auto rnd = randng::fast();
                if (rnd <= m_fields->chance) {
                    if (m_fields->scaled == true) {
                        log::debug("change scale big");
                        togglePlayerScale(m_fields->scaled, false);
                        m_fields->scaled = false;
                    } else {
                        log::debug("change scale small");
                        togglePlayerScale(m_fields->scaled, false);
                        m_fields->scaled = true;
                    };
                };
            };
        };

        return PlayerObject::pushButton(button);
    };
};