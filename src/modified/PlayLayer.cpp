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
        bool regen = player->m_isBird || player->m_isShip || player->m_isSwing || player->m_isDart;
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

    // do crap when player died
    void destroyPlayer(PlayerObject *player, GameObject *game)
    {
        auto horribleMod = getMod();
        if (!m_fields->m_destroyingObject)
            m_fields->m_destroyingObject = game;

        bool griefEnabled = horribleMod->getSavedValue("grief", true);
        bool congregEnabled = horribleMod->getSavedValue("congregation", true);
        bool wasDead = player ? player->m_isDead : true;

        PlayLayer::destroyPlayer(player, game);

        // get back to grief
        if (griefEnabled && player && !wasDead && player->m_isDead)
        {
            LevelManager::DownloadGriefLevel();
            // 10% chance to play grief level
            if (float griefChance = (rand() % 10) == 0)
            {
                auto glm = GameLevelManager::get();
                auto griefLevel = glm->getSavedLevel(105001928);

                if (griefLevel && !griefLevel->m_levelNotDownloaded && (!m_level || m_level->m_levelID.value() != 105001928))
                {
                    PlayLayer::destroyPlayer(player, game);
                    this->onExit();
                    auto scene = PlayLayer::scene(griefLevel, false, false);
                    CCDirector::get()->replaceScene(scene);
                    log::info("Switching to Grief level (105001928) by 10% chance");
                    return;
                }
                else if (griefLevel && !griefLevel->m_levelNotDownloaded)
                {
                    // Already in grief level, do nothing
                }
            }
            else
            {
                log::info("Grief jumpscare not triggered {}", griefChance);
            }
        }

        // congregation jumpscare
        if (congregEnabled && player && !wasDead && player->m_isDead)
        {
            LevelManager::DownloadCongregLevel();
            // 10% chance to play congregation level
            if (float CongregChance = (rand() % 10) == 0)
            {
                auto glm = GameLevelManager::get();
                auto congregLevel = glm->getSavedLevel(93437568);

                if (congregLevel && !congregLevel->m_levelNotDownloaded && (!m_level || m_level->m_levelID.value() != 93437568))
                {
                    PlayLayer::destroyPlayer(player, game);
                    this->onExit();
                    auto scene = PlayLayer::scene(congregLevel, false, false);
                    CCDirector::get()->replaceScene(scene);
                    log::info("Switching to Congregation level (93437568) by 10% chance");
                    return;
                }
                else if (congregLevel && !congregLevel->m_levelNotDownloaded)
                {
                    // Already in congregation level, do nothing
                }
            }
            else
            {
                log::info("Congregation jumpscare not triggered {}", CongregChance);
            }
        }
    }

    // new best
    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle)
    {
        auto horribleMod = getMod();
        int id = m_level->m_levelID;
        int percentage = m_level->m_normalPercent;

        log::info("Showing new best for level ID: {}", id);
        log::info("Level percentage: {}", percentage);

        PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);

        if (horribleMod->getSavedValue<bool>("mock", true) && percentage >= 90)
        {
            CCDirector *director = CCDirector::sharedDirector();
            CCScene *scene = CCScene::get();

            // Get the window size in points and scale to pixels
            auto winSize = director->getWinSize();
            int width = as<int>(winSize.width);
            int height = as<int>(winSize.height);

            CCRenderTexture *renderTexture = CCRenderTexture::create(width, height);

            renderTexture->begin();
            scene->visit();
            renderTexture->end();
            auto image = renderTexture->newCCImage();

            if (image)
            {
                std::string path = fmt::format("{}\\{}.png", Mod::get()->getSaveDir(), id);
                if (image->saveToFile(path.c_str(), false))
                {
                    log::info("Saved screenshot to {}", path);
                }
                else
                {
                    log::error("Failed to save screenshot to {}", path);
                }
                delete image;
            }
            else
            {
                log::error("Failed to create image from render texture");
            }
        }
    }
};