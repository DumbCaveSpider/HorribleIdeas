#include <Utils.hpp>
#include <Geode/modify/CCMenuItem.hpp>

using namespace horrible;

class $modify(AchievementCCMenuItem, CCMenuItem) {
    struct Fields {
        bool enabled = options::get("achieve");
        int chance = options::getChance("achieve");
    };

    void activate() {
        CCMenuItem::activate();

        if (m_fields->enabled) {
            if (auto fmod = FMODAudioEngine::sharedEngine()) {
                int rnd = randng::fast();
                log::debug("button menu chance {}", rnd);

                // @geode-ignore(unknown-resource)
                if (rnd <= m_fields->chance) fmod->playEffectAsync("achievement_01.ogg");
            };
        };
    };
};