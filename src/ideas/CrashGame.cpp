#include <Horrible.hpp>
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(CrashGamePlayLayer, PlayLayer)
{
    struct Fields
    {
        bool enabled = horribleMod->getSavedValue<bool>("crash_death", false);
        int chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("crash_death-chance"));
    };

    void destroyPlayer(PlayerObject *p0, GameObject *p1)
    {
        if (m_fields->enabled)
        {
            auto rnd = Rand::fast();
            log::debug("crash destroy chance {}", rnd);
            if (rnd <= m_fields->chance)
            {
                log::warn("ur game crash hehehehehehehe");
                PlayLayer::destroyPlayer(p0, p1);
                game::exit(true); // saves data
            }
        }

        PlayLayer::destroyPlayer(p0, p1);
    }
};