#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

struct Fields
{
    bool enabled = horribleMod->getSavedValue<bool>("placebo", false);
};

Fields m_fields;

void placeboEffect();

class $modify(PlaceboLevelPage, LevelPage)
{

    void onPlay(CCObject *sender)
    {
        placeboEffect();
        LevelPage::onPlay(sender);
    };
};

class $modify(PlaceboLevelInfoLayer, LevelInfoLayer)
{

    void onPlay(CCObject *sender)
    {
        placeboEffect();
        LevelInfoLayer::onPlay(sender);
    };
};



void placeboEffect()
{
    if (horribleMod && m_fields.enabled)
    {
        int rnd = Rand::tiny() % 1000;
        log::info("placebo effect roll: {}", rnd);

        if (rnd < 10)
        {
            log::info("Placebo effect activated! Enabling all horrible mod options...");

            for (const auto& option : modOptions)
            {
                horribleMod->setSavedValue<bool>(option.id, true);
                log::debug("Enabled option: {}", option.id);
            }
        }

        if (rnd < 5) {
            log::info("Placebo effect activated! Disabling all horrible mod options...");

            for (const auto& option : modOptions)
            {
                horribleMod->setSavedValue<bool>(option.id, false);
                log::debug("Disabled option: {}", option.id);
            }
        }
    };
};