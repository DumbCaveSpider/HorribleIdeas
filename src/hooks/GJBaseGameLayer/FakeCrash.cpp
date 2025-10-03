#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FakeCrashGJBaseGameLayer, GJBaseGameLayer)
{
    struct Fields {
        bool m_enabled = horribleMod->getSavedValue<bool>("fakeCrash", false);
        int m_chance = static_cast<int>(horribleMod->getSettingValue<int>("fakeCrash-chance"));
        float m_lastTimeWarp = LevelTools::getLastTimewarp();
        bool m_inFakeCrash = false;
        float m_fakeCrashStartTime = 0.0f;
    };
    void update(float p0)
    {
        if (auto playLayer = PlayLayer::get())
        {
            if (m_fields->m_enabled && !m_fields->m_inFakeCrash && rand() % m_fields->m_chance == 0)
            {
                log::debug("Faking crash");
                m_fields->m_lastTimeWarp = LevelTools::getLastTimewarp();
                GJBaseGameLayer::applyTimeWarp(0.0f);
                m_fields->m_inFakeCrash = true;
                m_fields->m_fakeCrashStartTime = playLayer->m_gameState.m_currentProgress;
            }

            if (m_fields->m_inFakeCrash)
            {
                float elapsedTime = playLayer->m_gameState.m_currentProgress - m_fields->m_fakeCrashStartTime;
                if (elapsedTime >= 5.0f)
                {
                    log::debug("Reverting timewarp to: {}", m_fields->m_lastTimeWarp);
                    GJBaseGameLayer::applyTimeWarp(m_fields->m_lastTimeWarp);
                    m_fields->m_inFakeCrash = false;
                }
            }
        }
        GJBaseGameLayer::update(p0);
    }
};