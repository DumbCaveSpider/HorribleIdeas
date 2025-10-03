#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(PlaceboPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("placebo", false);
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;
        if (m_fields->enabled)
        {
            int rnd = Rand::get(1000); // 0.05% chance
            log::debug("placebo init called {}", rnd);
            if (rnd < 1) // if rng is < 1
            {
                log::debug("placebo activated, you feel stronger");
                // make every options enabled
                for (auto& option : modOptions) {
                    horribleMod->setSavedValue<bool>(option.id, true);
                }
            }
            if (rnd < 5) {
                log::debug("placebo overactivated, you feel invincible");
                // make every options disabled
                for (auto& option : modOptions) {
                    horribleMod->setSavedValue<bool>(option.id, false);
                }
            }
        }
        return true;
    };
};