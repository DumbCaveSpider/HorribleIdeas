#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(ForceLevelsPlayLayer, PlayLayer) {
    struct Fields {
    bool enabled = horribleMod->getSavedValue<bool>("upside_down", false);
    int chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("upside_down-chance")); // chance as percent
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (horribleMod && m_fields->enabled) {
            float rng = Rand::fast(); // random float between 0 and 100
            log::debug("scene rng {} chance {}", rng, m_fields->chance);
            if (rng <= m_fields->chance) {
                log::debug("setting scene upside down");
                setRotation(-180.f);
            }
        }

        return true;
    }
};