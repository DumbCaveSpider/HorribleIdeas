#include <Geode/Geode.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Horrible.hpp>
#include <algorithm>

// 8 hours and still not working properly :(
// imma take a nap

using namespace geode::prelude;
using namespace horrible;

static GameObject* s_pendingKiller1 = nullptr;
static GameObject* s_pendingKiller2 = nullptr;
static PlayerObject* s_pendingPlayer1 = nullptr;
static PlayerObject* s_pendingPlayer2 = nullptr;
static bool s_parryActive1 = false;
static bool s_parryActive2 = false;
static float s_parryTimer1 = 0.f;
static float s_parryTimer2 = 0.f;
static bool s_parryScheduled1 = false;
static bool s_parryScheduled2 = false;

static PlayerObject* s_activeParryPlayer = nullptr;

constexpr float kParryWindow = 0.5f;  // seconds
class ParryPlayLayer;

class $modify(ParryPlayerObject, PlayerObject) {
      struct Fields {
            bool enabled = options::get("parry");
      };

      bool pushButton(PlayerButton p0) {
            const bool baseRet = PlayerObject::pushButton(p0);

            if (m_fields->enabled && p0 == PlayerButton::Jump) {
                  if (auto pl = PlayLayer::get()) {
                        if (this == pl->m_player1 && s_parryActive1) {
                              log::debug("canceling pending death for player 1");
                              s_parryActive1 = false;
                              s_parryTimer1 = 0.f;
                              s_pendingKiller1 = nullptr;
                              s_pendingPlayer1 = nullptr;
                              // hide UI
                              if (auto pl2 = PlayLayer::get()) {
                                    if (auto bar = static_cast<ProgressBar*>(pl2->getChildByID("parry"_spr))) bar->setVisible(false);
                                    if (auto lbl = static_cast<CCLabelBMFont*>(pl2->getChildByID("parry_label"_spr))) lbl->setVisible(false);
                                    s_activeParryPlayer = nullptr;
                              }
                              this->playSpawnEffect();
                              // play parry success sound
                              if (auto fmod = FMODAudioEngine::sharedEngine()) {
                                    fmod->playEffectAsync("chestClick.ogg");
                              }
                        } else if (this == pl->m_player2 && s_parryActive2) {
                              log::debug("canceling pending death for player 2");
                              s_parryActive2 = false;
                              s_parryTimer2 = 0.f;
                              s_pendingKiller2 = nullptr;
                              s_pendingPlayer2 = nullptr;
                              if (auto pl2 = PlayLayer::get()) {
                                    if (auto bar = static_cast<ProgressBar*>(pl2->getChildByID("parry"_spr))) bar->setVisible(false);
                                    if (auto lbl = static_cast<CCLabelBMFont*>(pl2->getChildByID("parry_label"_spr))) lbl->setVisible(false);
                                    s_activeParryPlayer = nullptr;
                              }
                              this->playSpawnEffect();
                              if (auto fmod = FMODAudioEngine::sharedEngine()) {
                                    fmod->playEffectAsync("chestClick.ogg");
                              }
                        }
                  }
            }

            return baseRet;
      }
};

class $modify(ParryPlayLayer, PlayLayer) {
      struct Fields {
            bool enabled = options::get("parry");
            Ref<ProgressBar> m_parryBar = nullptr;
            CCLabelBMFont* m_parryLabel = nullptr;
      };

      bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
            if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

            if (m_fields->enabled) {
                  if (!m_fields->m_parryBar) {
                        m_fields->m_parryBar = ProgressBar::create();
                        m_fields->m_parryBar->setID("parry"_spr);
                        m_fields->m_parryBar->setFillColor({255, 200, 0});
                        m_fields->m_parryBar->setAnchorPoint({0.5f, 0.5f});
                        m_fields->m_parryBar->setPosition({getScaledContentWidth() / 2.f, getScaledContentHeight() - 30.f});
                        m_fields->m_parryBar->setZOrder(10000);
                        m_fields->m_parryBar->setRotation(0.f);
                        m_fields->m_parryBar->setVisible(false);
                        addChild(m_fields->m_parryBar);
                        scheduleUpdate();
                  }

                  if (!m_fields->m_parryLabel) {
                        m_fields->m_parryLabel = CCLabelBMFont::create("PARRY", "bigFont.fnt");
                        m_fields->m_parryLabel->setColor({255, 200, 0});
                        m_fields->m_parryLabel->setAnchorPoint({0.5f, 0.5f});
                        m_fields->m_parryLabel->setScale(0.35f);
                        m_fields->m_parryLabel->setZOrder(10001);
                        m_fields->m_parryLabel->setPosition({getScaledContentWidth() / 2.f, getScaledContentHeight() - 24.f});
                        m_fields->m_parryLabel->setVisible(false);
                        m_fields->m_parryLabel->setID("parry_label"_spr);
                        addChild(m_fields->m_parryLabel, 101);
                  }
            }

            return true;
      }

      void showParryUI(PlayerObject* player) {
            if (!m_fields->enabled) return;
            if (player == m_player1) {
                  s_activeParryPlayer = player;
                  if (m_fields->m_parryBar) {
                        if (m_fields->m_parryBar->getParent() != this) {
                              m_fields->m_parryBar->removeFromParent();
                              addChild(m_fields->m_parryBar, 10000);
                              log::debug("parry UI reparented to PlayLayer for player1");
                        }
                        m_fields->m_parryBar->setVisible(true);
                        float pct = (s_parryTimer1 / kParryWindow) * 100.f;
                        if (pct < 0.f) pct = 0.f;
                        m_fields->m_parryBar->updateProgress(pct);
                        // convert player position to PlayLayer node space to correctly position the UI above the player
                        auto worldPos = player->convertToWorldSpaceAR({0.f, 0.f});
                        auto localPos = this->convertToNodeSpaceAR(worldPos);
                        m_fields->m_parryBar->setPosition({localPos.x, localPos.y + 30.f});
                  }
                  if (m_fields->m_parryLabel) {
                        if (m_fields->m_parryLabel->getParent() != this) {
                              m_fields->m_parryLabel->removeFromParent();
                              addChild(m_fields->m_parryLabel, 10001);
                              log::debug("parry label reparented to PlayLayer for player1");
                        }
                        std::string label = fmt::format("PARRY\n{:.2f}s", std::max(0.f, s_parryTimer1));
                        log::debug("showParryUI - player1, timer {}", s_parryTimer1);
                        m_fields->m_parryLabel->setString(label.c_str());
                        m_fields->m_parryLabel->setVisible(true);
                        auto worldPosL = player->convertToWorldSpaceAR({0.f, 0.f});
                        auto localPosL = this->convertToNodeSpaceAR(worldPosL);
                        m_fields->m_parryLabel->setPosition({localPosL.x, localPosL.y + 38.f});
                  }
            } else if (player == m_player2) {
                  s_activeParryPlayer = player;
                  if (m_fields->m_parryBar) {
                        if (m_fields->m_parryBar->getParent() != this) {
                              m_fields->m_parryBar->removeFromParent();
                              addChild(m_fields->m_parryBar, 10000);
                              log::debug("parry UI reparented to PlayLayer for player2");
                        }
                        m_fields->m_parryBar->setVisible(true);
                        float pct = (s_parryTimer2 / kParryWindow) * 100.f;
                        if (pct < 0.f) pct = 0.f;
                        m_fields->m_parryBar->updateProgress(pct);
                        auto worldPos = player->convertToWorldSpaceAR({0.f, 0.f});
                        auto localPos = this->convertToNodeSpaceAR(worldPos);
                        m_fields->m_parryBar->setPosition({localPos.x, localPos.y + 30.f});
                  }
                  if (m_fields->m_parryLabel) {
                        if (m_fields->m_parryLabel->getParent() != this) {
                              m_fields->m_parryLabel->removeFromParent();
                              addChild(m_fields->m_parryLabel, 10001);
                              log::debug("parry label reparented to PlayLayer for player2");
                        }
                        std::string label = fmt::format("PARRY\n{:.2f}s", std::max(0.f, s_parryTimer2));
                        log::debug("showParryUI - player2, timer {}", s_parryTimer2);
                        m_fields->m_parryLabel->setString(label.c_str());
                        m_fields->m_parryLabel->setVisible(true);
                        auto worldPosL = player->convertToWorldSpaceAR({0.f, 0.f});
                        auto localPosL = this->convertToNodeSpaceAR(worldPosL);
                        m_fields->m_parryLabel->setPosition({localPosL.x, localPosL.y + 38.f});
                  }
            }
      }

      void hideParryUI(PlayerObject* player) {
            if (!m_fields->enabled) return;
            // hide the shared UI and clear the active player if it matches
            if (!m_fields->m_parryBar) m_fields->m_parryBar->setVisible(false);
            if (m_fields->m_parryLabel) m_fields->m_parryLabel->setVisible(false);
            if (s_activeParryPlayer == player) s_activeParryPlayer = nullptr;
            // also unschedule any parry expire callbacks for this player
            if (player == m_player1) {
                  unschedule(schedule_selector(ParryPlayLayer::parryExpire1));
            } else if (player == m_player2) {
                  unschedule(schedule_selector(ParryPlayLayer::parryExpire2));
            }
      }

      void parryExpire1(float dt) {
            // unschedule ourselves
            unschedule(schedule_selector(ParryPlayLayer::parryExpire1));
            log::debug("parryExpire1 called dt={}", dt);
            if (!s_parryActive1) return;
            // finalize kill if still active
            PlayerObject* toKill = s_pendingPlayer1;
            GameObject* killer = s_pendingKiller1;
            s_parryActive1 = false;
            s_parryTimer1 = 0.f;
            s_pendingKiller1 = nullptr;
            s_pendingPlayer1 = nullptr;
            s_parryScheduled1 = false;
            if (m_fields->m_parryBar) m_fields->m_parryBar->setVisible(false);
            if (m_fields->m_parryLabel) m_fields->m_parryLabel->setVisible(false);
            s_activeParryPlayer = nullptr;
            if (toKill && killer) {
                  log::debug("parryExpire1: calling destroyPlayer (toKill={} killer={})", (uintptr_t)toKill, (uintptr_t)killer);
                  PlayLayer::destroyPlayer(toKill, killer);
                  if (!toKill->m_isDead) {
                        log::debug("parryExpire1: forcing death for player1");
                        toKill->m_isDead = true;
                        toKill->playDeathEffect();
                  }
            } else if (toKill && !killer) {
                  log::debug("parryExpire1: toKill set but killer null, forcing death");
                  toKill->m_isDead = true;
                  toKill->playDeathEffect();
            }
      }

      void parryExpire2(float dt) {
            unschedule(schedule_selector(ParryPlayLayer::parryExpire2));
            log::debug("parryExpire2 called dt={}", dt);
            if (!s_parryActive2) return;
            PlayerObject* toKill = s_pendingPlayer2;
            GameObject* killer = s_pendingKiller2;
            s_parryActive2 = false;
            s_parryTimer2 = 0.f;
            s_pendingKiller2 = nullptr;
            s_pendingPlayer2 = nullptr;
            s_parryScheduled2 = false;
            if (m_fields->m_parryBar) m_fields->m_parryBar->setVisible(false);
            if (m_fields->m_parryLabel) m_fields->m_parryLabel->setVisible(false);
            s_activeParryPlayer = nullptr;
            if (toKill && killer) {
                  log::debug("parryExpire2: calling destroyPlayer (toKill={} killer={})", (uintptr_t)toKill, (uintptr_t)killer);
                  PlayLayer::destroyPlayer(toKill, killer);
                  if (!toKill->m_isDead) {
                        log::debug("parryExpire2: forcing death for player2");
                        toKill->m_isDead = true;
                        toKill->playDeathEffect();
                  }
            } else if (toKill && !killer) {
                  log::debug("parryExpire2: toKill set but killer null, forcing death");
                  toKill->m_isDead = true;
                  toKill->playDeathEffect();
            }
      }

      void destroyPlayer(PlayerObject* player, GameObject* game) {
            if (!m_fields->enabled) {
                  PlayLayer::destroyPlayer(player, game);
                  return;
            }

            if (!game || (player && player->m_isDead)) {
                  PlayLayer::destroyPlayer(player, game);
                  return;
            }

            if (game == m_anticheatSpike && player && !player->m_isDead) {
                  return;
            }

            if (player == m_player1) {
                  // If already in parry window
                  if (s_parryActive1) {
                        return;
                  }

                  s_parryActive1 = true;
                  s_parryTimer1 = kParryWindow;
                  s_pendingKiller1 = game;
                  s_pendingPlayer1 = player;
                  showParryUI(player);
                  unschedule(schedule_selector(ParryPlayLayer::parryExpire1));
                  s_parryScheduled1 = true;
                  scheduleOnce(schedule_selector(ParryPlayLayer::parryExpire1), kParryWindow);
                  log::debug("Parry started for player 1 ({}s)", kParryWindow);
                  return;
            } else if (player == m_player2) {
                  // If already in parry window
                  if (s_parryActive2) {
                        return;
                  }

                  s_parryActive2 = true;
                  s_parryTimer2 = kParryWindow;
                  s_pendingKiller2 = game;
                  s_pendingPlayer2 = player;
                  showParryUI(player);
                  unschedule(schedule_selector(ParryPlayLayer::parryExpire2));
                  s_parryScheduled2 = true;
                  scheduleOnce(schedule_selector(ParryPlayLayer::parryExpire2), kParryWindow);
                  log::debug("Parry started for player 2 ({}s)", kParryWindow);
                  return;
            }
            PlayLayer::destroyPlayer(player, game);
      }

      void update(float dt) {
            PlayLayer::update(dt);

            if (!m_fields->enabled) return;
            log::debug("parry update dt: {} active1={} t1={} active2={} t2={}", dt, s_parryActive1, s_parryTimer1, s_parryActive2, s_parryTimer2);

            // Player 1
            if (s_parryActive1) {
                  // align UI over player 1
                  if (m_player1) {
                        float px = m_player1->getPositionX();
                        float py = m_player1->getPositionY();
                        if (m_fields->m_parryBar) {
                              auto worldPos = m_player1->convertToWorldSpaceAR({0.f, 0.f});
                              auto localPos = this->convertToNodeSpaceAR(worldPos);
                              m_fields->m_parryBar->setPosition({localPos.x, localPos.y + 30.f});
                        }
                        if (m_fields->m_parryLabel) {
                              auto worldPosL = m_player1->convertToWorldSpaceAR({0.f, 0.f});
                              auto localPosL = this->convertToNodeSpaceAR(worldPosL);
                              m_fields->m_parryLabel->setPosition({localPosL.x, localPosL.y + 38.f});
                        }
                        if (m_fields->m_parryBar) log::debug("parry UI parentIsPlayLayer: {} pos: {} {} visible: {}", m_fields->m_parryBar->getParent() == this, m_fields->m_parryBar->getPositionX(), m_fields->m_parryBar->getPositionY(), m_fields->m_parryBar->isVisible());
                  }
                  float oldTimer1 = s_parryTimer1;
                  s_parryTimer1 -= dt;
                  if (s_parryTimer1 != oldTimer1) {
                        log::debug("parry timer (p1): {} (pct {})", s_parryTimer1, (s_parryTimer1 / kParryWindow) * 100.f);
                  }
                  // update UI
                  if (m_fields->m_parryBar) {
                        float pct = (s_parryTimer1 / kParryWindow) * 100.f;
                        if (pct < 0.f) pct = 0.f;
                        m_fields->m_parryBar->updateProgress(pct);
                        log::debug("parry progress update: {}% visible: {}", pct, m_fields->m_parryBar->isVisible());
                  }
                  if (m_fields->m_parryLabel) {
                        std::string label = fmt::format("PARRY\n{:.2f}s", std::max(0.f, s_parryTimer1));
                        m_fields->m_parryLabel->setString(label.c_str());
                  }
                  if (s_parryTimer1 <= 0.f) {
                        // If we scheduled the expiration callback, let it do the finalize to avoid duplicate death calls
                        if (s_parryScheduled1) {
                              log::debug("parry timer <= 0 but scheduled callback will handle finalize (p1)");
                              // Ensure timer and active state are left for the scheduled callback
                              return;
                        }
                        log::debug("killing player1 toKill={} killer={}", (uintptr_t)s_pendingPlayer1, (uintptr_t)s_pendingKiller1);
                        // finalize the pending kill using base implementation
                        PlayerObject* toKill = s_pendingPlayer1;
                        GameObject* killer = s_pendingKiller1;
                        s_parryActive1 = false;
                        s_parryTimer1 = 0.f;
                        s_pendingKiller1 = nullptr;
                        s_pendingPlayer1 = nullptr;
                        if (m_fields->m_parryBar) m_fields->m_parryBar->setVisible(false);
                        if (m_fields->m_parryLabel) m_fields->m_parryLabel->setVisible(false);
                        s_activeParryPlayer = nullptr;
                        if (toKill && killer) {
                              bool wasDead = toKill->m_isDead;
                              log::debug("destroyPlayer call: wasDead={} ", wasDead);
                              PlayLayer::destroyPlayer(toKill, killer);
                              log::debug("destroyPlayer returned: isDead={} ", toKill->m_isDead);
                              // If another hook prevented the player from being destroyed  force death
                              if (!toKill->m_isDead) {
                                    log::debug("forcing death for player1");
                                    toKill->m_isDead = true;
                                    toKill->playDeathEffect();
                              }
                        } else if (toKill && !killer) {
                              log::debug("toKill set but killer null, forcing death for player1");
                              toKill->m_isDead = true;
                              toKill->playDeathEffect();
                        }
                  }
            }

            // Player 2
            if (s_parryActive2) {
                  // align UI over player 2
                  if (m_player2) {
                        float px = m_player2->getPositionX();
                        float py = m_player2->getPositionY();
                        if (m_fields->m_parryBar) {
                              auto worldPos = m_player2->convertToWorldSpaceAR({0.f, 0.f});
                              auto localPos = this->convertToNodeSpaceAR(worldPos);
                              m_fields->m_parryBar->setPosition({localPos.x, localPos.y + 30.f});
                        }
                        if (m_fields->m_parryLabel) {
                              auto worldPosL = m_player2->convertToWorldSpaceAR({0.f, 0.f});
                              auto localPosL = this->convertToNodeSpaceAR(worldPosL);
                              m_fields->m_parryLabel->setPosition({localPosL.x, localPosL.y + 38.f});
                        }
                        if (m_fields->m_parryBar) log::debug("parry UI parentIsPlayLayer: {} pos: {} {} visible: {}", m_fields->m_parryBar->getParent() == this, m_fields->m_parryBar->getPositionX(), m_fields->m_parryBar->getPositionY(), m_fields->m_parryBar->isVisible());
                  }

                  // Shared UI update: decide which player's timer to show
                  PlayerObject* uiPlayer = s_activeParryPlayer;
                  if (!uiPlayer) {
                        if (s_parryActive1)
                              uiPlayer = m_player1;
                        else if (s_parryActive2)
                              uiPlayer = m_player2;
                  }

                  if (uiPlayer && m_fields->m_parryBar && m_fields->m_parryLabel) {
                        float currentTimer = (uiPlayer == m_player1 ? s_parryTimer1 : s_parryTimer2);
                        float pct = (currentTimer / kParryWindow) * 100.f;
                        if (pct < 0.f) pct = 0.f;
                        m_fields->m_parryBar->updateProgress(pct);
                        m_fields->m_parryBar->setVisible(true);
                        m_fields->m_parryLabel->setVisible(true);
                        m_fields->m_parryBar->setPosition({uiPlayer->getPositionX(), uiPlayer->getPositionY() + 30.f});
                        m_fields->m_parryLabel->setPosition({uiPlayer->getPositionX(), uiPlayer->getPositionY() + 38.f});
                        std::string label = fmt::format("PARRY\n{:.2f}s", std::max(0.f, currentTimer));
                        m_fields->m_parryLabel->setString(label.c_str());
                  } else {
                        // no active UI - hide it
                        if (m_fields->m_parryBar) m_fields->m_parryBar->setVisible(false);
                        if (m_fields->m_parryLabel) m_fields->m_parryLabel->setVisible(false);
                  }
                  float oldTimer2 = s_parryTimer2;
                  s_parryTimer2 -= dt;
                  if (s_parryTimer2 != oldTimer2) {
                        log::debug("parry timer (p2): {} (pct {})", s_parryTimer2, (s_parryTimer2 / kParryWindow) * 100.f);
                  }
                  // update UI
                  if (m_fields->m_parryBar) {
                        float pct = (s_parryTimer2 / kParryWindow) * 100.f;
                        if (pct < 0.f) pct = 0.f;
                        m_fields->m_parryBar->updateProgress(pct);
                        log::debug("parry progress update: {}% visible: {}", pct, m_fields->m_parryBar->isVisible());
                  }
                  if (m_fields->m_parryLabel) {
                        std::string label = fmt::format("PARRY\n{:.2f}s", std::max(0.f, s_parryTimer2));
                        m_fields->m_parryLabel->setString(label.c_str());
                  }
                  if (s_parryTimer2 <= 0.f) {
                        // If we scheduled the expiration callback, let it do the finalize to avoid duplicate death calls
                        if (s_parryScheduled2) {
                              log::debug("parry timer <= 0 but scheduled callback will handle finalize (p2)");
                              return;
                        }
                        log::debug("killing player2 toKill={} killer={}", (uintptr_t)s_pendingPlayer2, (uintptr_t)s_pendingKiller2);
                        // Reset state
                        PlayerObject* toKill = s_pendingPlayer2;
                        GameObject* killer = s_pendingKiller2;
                        s_parryActive2 = false;
                        s_parryTimer2 = 0.f;
                        s_pendingKiller2 = nullptr;
                        s_pendingPlayer2 = nullptr;
                        if (m_fields->m_parryBar) m_fields->m_parryBar->setVisible(false);
                        if (m_fields->m_parryLabel) m_fields->m_parryLabel->setVisible(false);
                        s_activeParryPlayer = nullptr;
                        if (toKill && killer) {
                              bool wasDead = toKill->m_isDead;
                              log::debug("destroyPlayer call: wasDead={}", wasDead);
                              PlayLayer::destroyPlayer(toKill, killer);
                              log::debug("destroyPlayer returned: isDead={}", toKill->m_isDead);
                              if (!toKill->m_isDead) {
                                    log::debug("forcing death for player2");
                                    toKill->m_isDead = true;
                                    toKill->playDeathEffect();
                              }
                        } else if (toKill && !killer) {
                              log::debug("toKill set but killer null, forcing death for player2");
                              toKill->m_isDead = true;
                              toKill->playDeathEffect();
                        }
                  }
            }
      }

      void resetLevel() {
            // cleanup any pending parry state when resetting the level
            s_pendingKiller1 = nullptr;
            s_pendingKiller2 = nullptr;
            s_pendingPlayer1 = nullptr;
            s_pendingPlayer2 = nullptr;
            s_parryActive1 = false;
            s_parryActive2 = false;
            // ensure expire callbacks are unscheduled
            unschedule(schedule_selector(ParryPlayLayer::parryExpire1));
            unschedule(schedule_selector(ParryPlayLayer::parryExpire2));
            s_parryTimer1 = 0.f;
            s_parryTimer2 = 0.f;

            if (m_fields->m_parryBar) m_fields->m_parryBar->setVisible(false);
            if (m_fields->m_parryLabel) m_fields->m_parryLabel->setVisible(false);

            PlayLayer::resetLevel();
      }
};