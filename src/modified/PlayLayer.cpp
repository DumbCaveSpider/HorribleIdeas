#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayLayer.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>
#include "LevelManager.hpp"

using namespace geode::prelude;

class $modify(HorriblePlayLayer, PlayLayer)
{
    struct Fields
    {
        GameObject *m_destroyingObject;
        float m_oxygenLevel = 100.f;
        CCLabelBMFont *m_oxygenLabel = nullptr;
        bool m_oxygenActive = false;
    };

    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("achieve", true))
        {
            if (auto fmod = FMODAudioEngine::sharedEngine())
            {
                // @geode-ignore(unknown-resource)
                fmod->playEffectAsync("achievement_01.ogg");
            };
        }
        else
        {
            log::warn("Random achievements is disabled");
        };

        if (horribleMod->getSavedValue<bool>("oxygen", true))
        {
            if (level)
            {
                m_fields->m_oxygenActive = true;
                m_fields->m_oxygenLevel = 100.f;
                char buf[32];
                snprintf(buf, sizeof(buf), "Oxygen: %d", static_cast<int>(m_fields->m_oxygenLevel));
                if (!m_fields->m_oxygenLabel)
                {
                    m_fields->m_oxygenLabel = CCLabelBMFont::create(buf, "bigFont.fnt");
                    m_fields->m_oxygenLabel->setAnchorPoint({0.5f, 1.0f});
                    m_fields->m_oxygenLabel->setPosition({getContentSize().width / 2.f, getContentSize().height - 10.f});
                    m_fields->m_oxygenLabel->setScale(0.5f);
                    this->addChild(m_fields->m_oxygenLabel, 100);
                }
                else
                {
                    m_fields->m_oxygenLabel->setString(buf);
                }
                log::info("Oxygen level enabled for {}", level->m_levelName);

                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                    schedule_selector(HorriblePlayLayer::decreaseOxygen),
                    this, 0.0f, false);
            }
            else
            {
                log::warn("Oxygen level disabled for {}", level->m_levelName);
            }
        }

        if (horribleMod->getSavedValue<bool>("freeze", true))
        {
            if (auto gm = GameManager::sharedState())
            {
                // gm or ccdir dont have native set max fps methods
            };
        }
        else
        {
            log::warn("Random freezing at 90% is disabled");
        };

        if (horribleMod->getSavedValue<bool>("mock", true))
        {
            if (auto gm = GameManager::sharedState())
            {
                // use something like ccrendertexture to take screenshot
                auto renderTexture = CCRenderTexture::create(getContentSize().width, getContentSize().height);
                renderTexture->begin();
                visit();
                renderTexture->end();
                log::info("90% fail screenshot enabled");
            };
        }
        else
        {
            log::warn("Mocking 90% fail is disabled");
        };

        if (horribleMod->getSavedValue<bool>("grief", true))
        {
            log::info("Griefing is enabled");
            LevelManager::checkAndDownloadGriefLevel();
        }
        else
        {
            log::warn("Griefing is disabled");
        };

        return true;
    }

    void onExit()
    {
        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("achieve", true))
        {
            if (auto fmod = FMODAudioEngine::sharedEngine())
            {
                // @geode-ignore(unknown-resource)
                fmod->playEffectAsync("achievement_01.ogg");
            };
        }
        else
        {
            log::warn("Random achievements is disabled");
        };

        PlayLayer::onExit();
    };

    void decreaseOxygen(float dt)
    {
        if (!m_fields->m_oxygenActive || !m_fields->m_oxygenLabel)
            return;
        auto player = this->m_player1;
        if (!player)
            return;
        bool regen = player->m_isBird || player->m_isShip || player->m_isSwing;
        if (regen)
        {
            m_fields->m_oxygenLevel += 5.f * dt;
            // log::debug("Oxygen level increased: {}", m_fields->m_oxygenLevel);
        }
        else
        {
            m_fields->m_oxygenLevel -= 2.f * dt;
            // log::debug("Oxygen level decreased: {}", m_fields->m_oxygenLevel);
        }
        if (m_fields->m_oxygenLevel > 100.f)
            m_fields->m_oxygenLevel = 100.f;
        if (m_fields->m_oxygenLevel < 0.f)
            m_fields->m_oxygenLevel = 0.f;
        char buf[32];
        snprintf(buf, sizeof(buf), "Oxygen: %d", static_cast<int>(m_fields->m_oxygenLevel));
        m_fields->m_oxygenLabel->setString(buf);
        // Destroy player if oxygen is 0
        if (m_fields->m_oxygenLevel <= 0.f && m_player1 && !m_player1->m_isDead)
        {
            this->destroyPlayer(m_player1, nullptr);
        }
    }

    void resetOxygenLevel()
    {
        m_fields->m_oxygenLevel = 100.f;
        if (m_fields->m_oxygenLabel)
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "Oxygen: %d", static_cast<int>(m_fields->m_oxygenLevel));
            m_fields->m_oxygenLabel->setString(buf);
        }
    }

    void resetLevel()
    {
        resetOxygenLevel();
        PlayLayer::resetLevel();
    }

    void onRestart()
    {
        resetOxygenLevel();
    }

    void destroyPlayer(PlayerObject *player, GameObject *game)
    {
        auto horribleMod = getMod();
        if (!m_fields->m_destroyingObject)
            m_fields->m_destroyingObject = game;

        bool griefEnabled = horribleMod->getSavedValue("grief", true);
        bool wasDead = player ? player->m_isDead : true;

        PlayLayer::destroyPlayer(player, game);

        if (griefEnabled && player && !wasDead && player->m_isDead)
        {
            auto glm = GameLevelManager::get();
            auto griefLevel = glm->getSavedLevel(105001928);
            LevelManager::checkAndDownloadGriefLevel();

            if (griefLevel && !griefLevel->m_levelNotDownloaded && (!m_level || m_level->m_levelID.value() != 105001928))
            {
                this->onExit();
                auto scene = PlayLayer::scene(griefLevel, false, false);
                CCDirector::get()->replaceScene(scene);
                log::info("Switching to Grief level (105001928)");
                return;
            }
            else if (griefLevel && !griefLevel->m_levelNotDownloaded)
            {
                // Already in grief level, do nothing
            }
        }
    }
};