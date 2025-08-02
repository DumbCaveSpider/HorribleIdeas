#include "../utils/LevelManager.hpp"
#include "../popups/RandomAdPopup.hpp"

#include <ctime>
#include <cstdlib>

#include <fmt/core.h>

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayLayer.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>

struct RandomSeeder
{
    RandomSeeder() { srand(time(0)); }
};

using namespace geode::prelude;
using namespace geode::utils;
using namespace matjson;

class $modify(HorriblePlayLayer, PlayLayer)
{
    struct Fields
    {
        GameObject *m_destroyingObject;

        RandomAdPopup *m_currentAd = nullptr;

        CCLabelBMFont *m_healthLabel = nullptr;
        CCLabelBMFont *m_oxygenLabel = nullptr;

        float m_health = 100.f;
        float m_oxygenLevel = 100.f;
        bool m_oxygenActive = false;

        Ref<CCSprite> m_oxygenBar;
        CCSprite *m_oxygenBarFill;

        Ref<CCSprite> m_healthBar;
        CCSprite *m_healthBarFill;
    };

    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects)
    {
        srand(time(0));
        int rnd = rand() % 101;
        log::info("playlayer init called {}", rnd);

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        auto horribleMod = getMod();
        if (horribleMod && horribleMod->getSavedValue<bool>("black-screen", false))
        {
            log::debug("black screen enabled, init scheduling black screen");

            float delay = static_cast<float>(rand() % 10001) / 1000.0f; // random delay between 0 and 10 seconds
            log::debug("Black screen will appear after {} seconds", delay);

            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(HorriblePlayLayer::showBlackScreen),
                this, delay, false);
        }

        if (horribleMod && horribleMod->getSavedValue<bool>("upside-down", false))
        {
            log::debug("scene rng {} chance", rnd);
            if (rnd <= 50)
            {
                log::debug("setting scene upside down");
                this->setRotation(-180.f);
            }
        }

        if (horribleMod->getSavedValue<bool>("health", false))
        {
            m_fields->m_health = 100.f;
            std::string hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));

            if (!m_fields->m_healthBar) {
                m_fields->m_healthBar = CCSprite::create("slidergroove2.png");
                m_fields->m_healthBar->setID("health"_spr);
                m_fields->m_healthBar->setPosition({10.f, getScaledContentHeight() / 2.f});
                m_fields->m_healthBar->setRotation(90.f);
                m_fields->m_healthBar->setZOrder(101);

                m_fields->m_healthBarFill = CCSprite::create("sliderBar2.png");
                m_fields->m_healthBarFill->setID("health-fill"_spr);
                m_fields->m_healthBarFill->setZOrder(-1);
                m_fields->m_healthBarFill->setRotation(-180.f);
                m_fields->m_healthBarFill->setColor({255, 0, 0});
                m_fields->m_healthBarFill->setPosition({m_fields->m_healthBar->getScaledContentWidth() - 2.f, 4.f});
                m_fields->m_healthBarFill->setAnchorPoint({0, 1});

                auto fullHealth = m_fields->m_healthBar->getScaledContentWidth() - 4.f;

                m_fields->m_healthBarFill->setTextureRect({0, 0, fullHealth, 8});

                m_fields->m_healthBar->addChild(m_fields->m_healthBarFill);

                addChild(m_fields->m_healthBar);
            }

            if (!m_fields->m_healthLabel)
            {
                m_fields->m_healthLabel = CCLabelBMFont::create(hp.c_str(), "bigFont.fnt");
                m_fields->m_healthLabel->setColor({255, 0, 0});
                m_fields->m_healthLabel->setAnchorPoint({0.f, 1.0f});
                m_fields->m_healthLabel->setPosition({2.f, (getScaledContentHeight() / 2.f) - (m_fields->m_healthBar->getScaledContentWidth() / 2.f) - 1.25f});
                m_fields->m_healthLabel->setScale(0.25f);

                addChild(m_fields->m_healthLabel, 100);
            }
            else
            {
                m_fields->m_healthLabel->setString(hp.c_str());
            };
        };

        if (horribleMod->getSavedValue<bool>("oxygen", false))
        {
            m_fields->m_oxygenActive = true;
            m_fields->m_oxygenLevel = 100.f;

            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(HorriblePlayLayer::decreaseOxygen),
                this, 0.0f, false);

            if (!m_fields->m_oxygenBar)
            {
                m_fields->m_oxygenBar = CCSprite::create("slidergroove2.png");
                m_fields->m_oxygenBar->setID("oxygen"_spr);
                m_fields->m_oxygenBar->setPosition({10.f, getScaledContentHeight() / 2.f});
                m_fields->m_oxygenBar->setRotation(90.f);
                m_fields->m_oxygenBar->setZOrder(101);

                m_fields->m_oxygenBarFill = CCSprite::create("sliderBar2.png");
                m_fields->m_oxygenBarFill->setID("oxygen-fill"_spr);
                m_fields->m_oxygenBarFill->setZOrder(-1);
                m_fields->m_oxygenBarFill->setRotation(-180.f);
                m_fields->m_oxygenBarFill->setColor({0, 175, 255});
                m_fields->m_oxygenBarFill->setPosition({m_fields->m_oxygenBar->getScaledContentWidth() - 2.f, 4.f});
                m_fields->m_oxygenBarFill->setAnchorPoint({0, 1});

                m_fields->m_oxygenBarFill->setTextureRect({0, 0, 0, 8});

                m_fields->m_oxygenBar->addChild(m_fields->m_oxygenBarFill);

                addChild(m_fields->m_oxygenBar);
            };

            if (m_fields->m_healthBar)
            {
                m_fields->m_oxygenBar->setPosition({m_fields->m_healthBar->getPositionX() + 20.f, m_fields->m_healthBar->getPositionY()});
            }
            else
            {
                m_fields->m_oxygenBar->setPosition({10.f, getScaledContentHeight() / 2.f});
            };

            std::string buf = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));

            if (!m_fields->m_oxygenLabel)
            {
                m_fields->m_oxygenLabel = CCLabelBMFont::create(buf.c_str(), "bigFont.fnt");
                m_fields->m_oxygenLabel->setColor({0, 175, 255});
                m_fields->m_oxygenLabel->setAnchorPoint({0.f, 1.0f});
                m_fields->m_oxygenLabel->setPosition({2.f, (getScaledContentHeight() / 2.f) - (m_fields->m_oxygenBar->getScaledContentWidth() / 2.f) - 1.25f});
                m_fields->m_oxygenLabel->setScale(0.25f);

                addChild(m_fields->m_oxygenLabel, 100);
            }
            else
            {
                m_fields->m_oxygenLabel->setString(buf.c_str());
            };

            if (m_fields->m_healthLabel) {
                m_fields->m_oxygenLabel->setPosition({m_fields->m_oxygenLabel->getPositionX() + 20.f, m_fields->m_oxygenLabel->getPositionY()});
            }
            else
            {
                m_fields->m_oxygenLabel->setPosition({2.f, (getScaledContentHeight() / 2.f) - (m_fields->m_oxygenBar->getScaledContentWidth() / 2.f) - 1.25f});
            }
        }
        else
        {
            log::warn("Oxygen meter is disabled");
        };

        if (horribleMod->getSavedValue<bool>("freeze", false))
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
    };

    void update(float p0)
    {
        log::debug("update called");

        auto rnd = rand() % 101;
        log::debug("playlayer update chance {}", rnd);

        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("math-quiz", false))
        {
            if (rnd <= 1)
            {
                if (m_isPracticeMode)
                    log::warn("richard was here");
            };
        };

        if (horribleMod->getSavedValue<bool>("ads", false))
        {
            if (rnd <= 5)
            {
                log::warn("ad time!");

                if (!m_fields->m_currentAd)
                {
                    if (auto popup = RandomAdPopup::create())
                    {
                        m_fields->m_currentAd = popup;
                        m_fields->m_currentAd->show();
                    };
                };
            };
        };

        // Parry logic: constant check every frame
        if (horribleMod->getSavedValue<bool>("parry", false))
        {
            log::debug("parry is real");

            auto playerRect = m_player1->getObjectRect();
            if (m_objects)
            {
                for (int i = 0; i < m_objects->count(); ++i)
                {
                    auto obj = static_cast<GameObject *>(m_objects->objectAtIndex(i));
                    if (obj == m_anticheatSpike)
                    {
                        log::debug("Skipping anticheat spike object");
                        continue;
                    }

                    if (obj && playerRect.intersectsRect(obj->getObjectRect()))
                    {
                        log::debug("Player is inside an obstacle hitbox: {} (damage would be applied)", obj->getID());
                        destroyPlayer(nullptr, nullptr);
                    }
                    else
                    {
                        log::debug("Player is not inside an obstacle hitbox");
                    }
                }
            }
        };

        PlayLayer::update(p0);
    };

    void pauseGame(bool p0)
    {
        revertFPS();
        PlayLayer::pauseGame(p0);
    }

    void revertFPS()
    {
        // default to user old fps
        auto gm = GameManager::get();
        float oldFPS = Mod::get()->getSavedValue<float>("fps");
        gm->setGameVariable("0116", true);

        // Use seconds per frame, not raw FPS
        float interval = (oldFPS > 10.f) ? (1.f / oldFPS) : (1.f / 60.f); // minimum 10 FPS
        if (interval <= 0.0f || interval > 1.0f)
            interval = 1.f / 60.f; // fallback to 60 FPS if invalid
        CCDirector::sharedDirector()->setAnimationInterval(interval);
        log::debug("reset fps to {} (interval {})", oldFPS, interval);
    }

    void capFPS(float value)
    {
        auto gm = GameManager::get();
        float oldFPS = Mod::get()->getSavedValue<float>("fps");

        gm->setGameVariable("0116", true);

        // cap fps to 60
        float interval = 1.f / value;
        if (interval <= 0.0f || interval > 1.0f)
            interval = 1.f / 60.f; // fallback to 60 FPS if invalid
        CCDirector::sharedDirector()->setAnimationInterval(interval);
        log::debug("cap fps to {} (interval {})", value, interval);
    }

    void onQuit()
    {

        revertFPS();

        // achievement
        if (Mod::get()->getSavedValue<bool>("achieve", false))
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

        PlayLayer::onQuit();
    };

    void showBlackScreen(float)
    {
        auto horribleMod = getMod();
        if (horribleMod->getSavedValue<bool>("black-screen", false))
        {
            log::debug("Showing black screen after delay");
            auto blackScreen = CCScale9Sprite::create("square02_001.png");
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            blackScreen->setContentSize({winSize.width + 10.f, winSize.height + 10.f});
            blackScreen->setPosition({winSize.width / 2.f, winSize.height / 2.f});
            blackScreen->setID("black-screen"_spr);
            blackScreen->setZOrder(100);

            addChild(blackScreen);

            // Schedule removal after 0.5 seconds, then schedule to show again after a random delay
            blackScreen->runAction(
                CCSequence::create(
                    CCDelayTime::create(0.25f),
                    CCCallFunc::create(this, callfunc_selector(HorriblePlayLayer::removeBlackScreen)),
                    nullptr));
        }
    };

    void removeBlackScreen()
    {
        auto blackScreen = this->getChildByID("black-screen"_spr);
        auto horribleMod = getMod();
        if (blackScreen)
        {
            blackScreen->removeFromParent();
            log::debug("Black screen removed");

            if (horribleMod->getSavedValue<bool>("black-screen", false))
            {

                float delay = static_cast<float>(rand() % 10001) / 1000.0f; // random delay between 0 and 10 seconds
                log::debug("Black screen will appear again after {} seconds", delay);

                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                    schedule_selector(HorriblePlayLayer::showBlackScreen),
                    this, delay, false);
            }
            else
            {
                log::warn("No black screen found to remove");
            }
        }
    };

    void decreaseOxygen(float dt)
    {
        if (!m_fields->m_oxygenActive || !m_fields->m_oxygenLabel)
            return;
        auto player = m_player1;
        if (!player)
            return;

        // regen o2
        if (player->m_isBird || player->m_isShip || player->m_isSwing || player->m_isDart)
        {
            m_fields->m_oxygenLevel += 5.f * dt;
            // log::debug("Oxygen level increased: {}", m_fields->m_oxygenLevel);
        }
        else
        {
            m_fields->m_oxygenLevel -= 2.f * dt;
            // log::debug("Oxygen level decreased: {}", m_fields->m_oxygenLevel);
        };

        if (m_fields->m_oxygenLevel > 100.f)
            m_fields->m_oxygenLevel = 100.f;
        if (m_fields->m_oxygenLevel < 0.f)
            m_fields->m_oxygenLevel = 0.f;

        std::string buf = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));
        m_fields->m_oxygenLabel->setString(buf.c_str());

        float maxWidth = m_fields->m_oxygenBar->getContentWidth() - 4.f;
        float newWidth = maxWidth * (m_fields->m_oxygenLevel / 100.f);

        m_fields->m_oxygenBarFill->setTextureRect({0, 0, newWidth, 8});

        // Destroy player if oxygen is 0
        if (m_fields->m_oxygenLevel <= 0.f && m_player1 && !m_player1->m_isDead)
            destroyPlayer(m_player1, nullptr);
    };

    void resetOxygenLevel()
    {
        m_fields->m_oxygenLevel = 100.f;

        if (m_fields->m_oxygenLabel)
        {
            std::string buf = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));
            m_fields->m_oxygenLabel->setString(buf.c_str());
        };
    };

    void resetHealth()
    {
        m_fields->m_health = 100.f;

        if (m_fields->m_healthLabel)
        {
            std::string hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));
            m_fields->m_healthLabel->setString(hp.c_str());
        }

        if (m_fields->m_healthBar)
        {
            float maxWidth = m_fields->m_healthBar->getContentWidth() - 4.f;
            m_fields->m_healthBarFill->setTextureRect({0, 0, maxWidth, 8});
        }
    }

    void resetLevel()
    {
        resetOxygenLevel();
        resetHealth();
        PlayLayer::resetLevel();
    };

    // do crap when player died
    void destroyPlayer(PlayerObject *player, GameObject *game)
    {
        if (game == m_anticheatSpike && player && !player->m_isDead)
        {
            // ignore the anticheat spike lmao
            return;
        }

        auto rnd = rand() % 101;
        log::debug("destroy chance {}", rnd);

        auto horribleMod = getMod();
        if (!m_fields->m_destroyingObject)
            m_fields->m_destroyingObject = game;

        bool crashEnabled = horribleMod->getSavedValue("crash-death", false);
        bool griefEnabled = horribleMod->getSavedValue("grief", false);
        bool congregEnabled = horribleMod->getSavedValue("congregation", false);
        bool fakeDeadEnabled = horribleMod->getSavedValue("death", false);
        bool healthEnabled = horribleMod->getSavedValue("health", false);

        // do stuff for health
        if (healthEnabled)
        {
            log::debug("health enabled, checking player health");
            if (m_fields->m_health > 0)
            {
                m_fields->m_health -= 0.1f;
                log::debug("Player health is {}", m_fields->m_health);
                m_player1->playSpawnEffect();
                m_player2->playSpawnEffect();
                GJBaseGameLayer::shakeCamera(1.f, 5.f, 1.f);

                if (rand() % 2 == 0)
                {
                    FMODAudioEngine::sharedEngine()->playEffect("grunt01.ogg");
                } else if (rand() % 2 == 1)
                {
                    FMODAudioEngine::sharedEngine()->playEffect("grunt02.ogg");
                } else if (rand() % 2 == 2)
                {
                    FMODAudioEngine::sharedEngine()->playEffect("grunt03.ogg");
                };

                if (m_fields->m_healthLabel)
                {
                    std::string hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));
                    m_fields->m_healthLabel->setString(hp.c_str());

                    // Update health bar
                    float maxWidth = m_fields->m_healthBar->getContentWidth() - 4.f;
                    float newWidth = maxWidth * (m_fields->m_health / 100.f);
                    m_fields->m_healthBarFill->setTextureRect({0, 0, newWidth, 8});
                }
                return;
            }
            else
            {
                log::debug("Player health is 0, destroying player");
                PlayLayer::destroyPlayer(player, game);
            }
        }

        // Show explosion visual effect but do not kill the player
        if (fakeDeadEnabled)
        {
            // @geode-ignore(unknown-resource)
            FMODAudioEngine::sharedEngine()->playEffect("explode_11.ogg");
            GJBaseGameLayer::shakeCamera(1.f, 2.f, 1.f);

            if (m_player1)
            {
                log::debug("fake death");
                m_player1->playDeathEffect();
                m_player1->resetPlayerIcon();
                m_player1->m_isDead = false;

                m_player2->playDeathEffect();
                m_player2->resetPlayerIcon();
                m_player2->m_isDead = false;
            }
        }
        else if (!fakeDeadEnabled)
        {
            log::debug("real death");
            PlayLayer::destroyPlayer(player, game);
        }

        if (crashEnabled)
        {
            if (rnd <= 1)
            {
                log::warn("ur game crash hehehehehehehe");
                PlayLayer::destroyPlayer(player, game);
                game::exit(true); // saves data
            }
            else
            {
                log::debug("ur safe from crash... for now");
                PlayLayer::destroyPlayer(player, game);
            };
        };

        // get back to grief
        if (griefEnabled)
        {
            LevelManager::DownloadGriefLevel();
            // 10% chance to play grief level
            if (float griefChance = rnd <= 10)
            {
                auto glm = GameLevelManager::get();
                auto griefLevel = glm->getSavedLevel(105001928);

                if (griefLevel && !griefLevel->m_levelNotDownloaded && (!m_level || m_level->m_levelID.value() != 105001928))
                {
                    PlayLayer::destroyPlayer(player, game);

                    onExit();

                    auto scene = PlayLayer::scene(griefLevel, false, false);
                    CCDirector::get()->replaceScene(scene);

                    log::info("Switching to Grief level (105001928) by 10% chance");

                    return;
                }
                else if (griefLevel && !griefLevel->m_levelNotDownloaded)
                {
                    // Already in grief level, do nothing
                    PlayLayer::destroyPlayer(player, game);
                };
            }
            else
            {
                log::info("Grief jumpscare not triggered {}", griefChance);
                PlayLayer::destroyPlayer(player, game);
            };
        };

        // congregation jumpscare
        if (congregEnabled)
        {
            LevelManager::DownloadCongregLevel();
            // 10% chance to play congregation level
            if (float CongregChance = rnd <= 10)
            {
                auto glm = GameLevelManager::get();
                auto congregLevel = glm->getSavedLevel(93437568);

                if (congregLevel && !congregLevel->m_levelNotDownloaded && (!m_level || m_level->m_levelID.value() != 93437568))
                {
                    PlayLayer::destroyPlayer(player, game);

                    onExit();

                    auto scene = PlayLayer::scene(congregLevel, false, false);
                    CCDirector::get()->replaceScene(scene);

                    log::info("Switching to Congregation level (93437568) by 10% chance");
                    return;
                }
                else if (congregLevel && !congregLevel->m_levelNotDownloaded)
                {
                    // Already in congregation level, do nothing
                    PlayLayer::destroyPlayer(player, game);
                };
            }
            else
            {
                log::info("Congregation jumpscare not triggered {}", CongregChance);
                PlayLayer::destroyPlayer(player, game);
            };
        };
    };

    // new best
    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle)
    {
        auto horribleMod = getMod();

        int id = m_level->m_levelID;
        int percentage = m_level->m_normalPercent;

        log::info("Showing new best for level ID: {}", id);
        log::info("Level percentage: {}", percentage);

        PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);

#if defined(GEODE_IS_MACOS) && defined(GEODE_IS_IOS)
        Notification::create("Mock is not avaliable on this platform", NotificationIcon::Info, 1.f);
#endif

#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS) // not available for these platforms
        if (horribleMod->getSavedValue<bool>("mock", false) && percentage >= 90)
        {
            CCDirector *director = CCDirector::sharedDirector();
            CCScene *scene = CCScene::get();

            // Get the window size in points and scale to pixels
            auto winSize = director->getWinSize();

            int width = static_cast<int>(winSize.width);
            int height = static_cast<int>(winSize.height);

            CCRenderTexture *renderTexture = CCRenderTexture::create(width, height);

            renderTexture->begin();
            scene->visit();
            renderTexture->end();

            if (auto image = renderTexture->newCCImage())
            {
                std::string path = fmt::format("{}\\{}.png", horribleMod->getSaveDir(), id);

                if (image->saveToFile(path.c_str(), false))
                {
                    auto mockConfigPath = fmt::format("{}\\mock.json", horribleMod->getSaveDir());
                    auto mockConfig = file::readJson(std::filesystem::path(mockConfigPath)); // get the saved fails to mock the player with :)

                    auto toWrite = Value(); // what we're gonna write in the mock.json file

                    if (mockConfig.isOk())
                    {
                        // unwrap the whole thing
                        auto mockConfigUnwr = mockConfig.unwrapOr(Value());

                        // overwrite this field (or add it) with the percent
                        mockConfigUnwr[std::to_string(id)] = percentage;

                        toWrite = mockConfigUnwr;
                    }
                    else
                    {
                        toWrite = makeObject({{std::to_string(id), percentage}});
                    };

                    if (!toWrite.isNull())
                    {
                        auto mockJson = file::writeToJson(mockConfigPath, toWrite);

                        if (mockJson.isOk())
                        {
                            log::info("Saved highly mockable percentage of {} to data", percentage);
                        }
                        else
                        {
                            log::error("Aw man, failed to save mockable percentage of {} to data", percentage);
                        };
                    };

                    log::info("Saved screenshot to {}", path);
                }
                else
                {
                    log::error("Failed to save screenshot to {}", path);
                };

                CC_SAFE_DELETE(image);
            }
            else
            {
                log::error("Failed to create image from render texture");
            };
        };
#endif
    };

    void levelComplete()
    {
        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("mock", false))
        {
            int id = m_level->m_levelID;
            int percentage = m_level->m_normalPercent;

            auto mockConfigPath = fmt::format("{}\\mock.json", horribleMod->getSaveDir());
            auto mockConfig = file::readJson(std::filesystem::path(mockConfigPath)); // get the saved levels to mock the player :)

            if (mockConfig.isOk())
            {
                log::debug("Clearing mock record for {}", id);
                auto mockConfigUnwr = mockConfig.unwrapOr(Value());

                mockConfigUnwr[std::to_string(id)].clear();

                auto mockJson = file::writeToJson(mockConfigPath, mockConfigUnwr);

                if (mockJson.isOk())
                {
                    log::info("Saved highly mockable percentage of {} to data", percentage);
                }
                else
                {
                    log::error("Aw man, failed to save mockable percentage of {} to data", percentage);
                };
            };
        };

        PlayLayer::levelComplete();
    };
};