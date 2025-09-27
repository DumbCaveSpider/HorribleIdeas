#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(CrashGamePlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = HorribleIdeas::get("crash_death");
        int chance = HorribleIdeas::getChance("crash_death");

        bool m_dontCreateObjects = false;
        GameObject* m_destroyingObject;
    };

    void destroyPlayer(PlayerObject * p0, GameObject * p1) {
        if (m_fields->enabled) {
            // ignore the anti-cheat spike lmao
            if (p1 == m_anticheatSpike && p0 && !p0->m_isDead) return;
            if (!m_fields->m_destroyingObject) m_fields->m_destroyingObject = p1;

            auto rnd = Rand::fast();
            log::debug("crash destroy chance {}", rnd);

            if (rnd <= m_fields->chance) {
                log::warn("ur game crash hehehehehehehe");

                PlayLayer::destroyPlayer(p0, p1);
                game::exit(true); // saves data
            };
        };

        PlayLayer::destroyPlayer(p0, p1);
    };
};