#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(GamblerPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("gambler", false);
    };

    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;
        if (m_fields->enabled)
        {
            log::debug("gambler enabled");
        }
        return true;
    };
};