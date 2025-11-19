#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HealthBarPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("health");

        CCLabelBMFont* m_healthLabel = nullptr;

        float m_health = 100.f;

        Ref<CCSprite> m_healthBar;
        CCSprite* m_healthBarFill;

        bool m_dontCreateObjects = false;
        GameObject* m_destroyingObject;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (m_fields->enabled) {
            m_fields->m_health = 100.f;
            std::string hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));

            if (!m_fields->m_healthBar) {
                m_fields->m_healthBar = CCSprite::create("slidergroove2.png");
                m_fields->m_healthBar->setID("health"_spr);
                m_fields->m_healthBar->setPosition({ 10.f, getScaledContentHeight() / 2.f });
                m_fields->m_healthBar->setRotation(90.f);
                m_fields->m_healthBar->setZOrder(101);

                m_fields->m_healthBarFill = CCSprite::create("sliderBar2.png");
                m_fields->m_healthBarFill->setID("health-fill"_spr);
                m_fields->m_healthBarFill->setZOrder(-1);
                m_fields->m_healthBarFill->setRotation(-180.f);
                m_fields->m_healthBarFill->setColor({ 255, 0, 0 });
                m_fields->m_healthBarFill->setPosition({ m_fields->m_healthBar->getScaledContentWidth() - 2.f, 4.f });
                m_fields->m_healthBarFill->setAnchorPoint({ 0, 1 });

                auto fullHealth = m_fields->m_healthBar->getScaledContentWidth() - 4.f;

                m_fields->m_healthBarFill->setTextureRect({ 0, 0, fullHealth, 8 });

                m_fields->m_healthBar->addChild(m_fields->m_healthBarFill);

                addChild(m_fields->m_healthBar);
            };

            if (!m_fields->m_healthLabel) {
                m_fields->m_healthLabel = CCLabelBMFont::create(hp.c_str(), "bigFont.fnt");
                m_fields->m_healthLabel->setColor({ 255, 0, 0 });
                m_fields->m_healthLabel->setAnchorPoint({ 0.f, 1.0f });
                m_fields->m_healthLabel->setPosition({ 2.f, (getScaledContentHeight() / 2.f) - (m_fields->m_healthBar->getScaledContentWidth() / 2.f) - 1.25f });
                m_fields->m_healthLabel->setScale(0.25f);

                addChild(m_fields->m_healthLabel, 100);
            } else {
                m_fields->m_healthLabel->setString(hp.c_str());
            };
        };

        return true;
    };

    void resetHealth() {
        m_fields->m_health = 100.f;

        if (m_fields->m_healthLabel) {
            std::string hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));
            m_fields->m_healthLabel->setString(hp.c_str());
        };

        if (m_fields->m_healthBar) {
            float maxWidth = m_fields->m_healthBar->getContentWidth() - 4.f;
            m_fields->m_healthBarFill->setTextureRect({ 0, 0, maxWidth, 8 });
        };
    };

    void resetLevel() {
        resetHealth();
        PlayLayer::resetLevel();
    };

    void destroyPlayer(PlayerObject * player, GameObject * game) {
        if (!m_fields->enabled) {
            PlayLayer::destroyPlayer(player, game);
        } else if (m_fields->enabled) {
            // ignore the anti-cheat spike lmao
            if (game == m_anticheatSpike && player && !player->m_isDead) return;
            if (!m_fields->m_destroyingObject) m_fields->m_destroyingObject = game;

            auto rnd = randng::fast();
            if (m_fields->m_health > 0) {
                m_fields->m_health -= 0.1f;

                // log::debug("Player health is {}", m_fields->m_health);

                m_player1->playSpawnEffect();
                m_player2->playSpawnEffect();

                GJBaseGameLayer::shakeCamera(1.f, 5.f, 1.f);

                if (rnd % 2 == 0) {
                    // @geode-ignore(unknown-resource)
                    FMODAudioEngine::sharedEngine()->playEffect("grunt01.ogg");
                } else if (rnd % 2 == 1) {
                    // @geode-ignore(unknown-resource)
                    FMODAudioEngine::sharedEngine()->playEffect("grunt02.ogg");
                } else if (rnd % 2 == 2) {
                    // @geode-ignore(unknown-resource)
                    FMODAudioEngine::sharedEngine()->playEffect("grunt03.ogg");
                };

                if (m_fields->m_healthLabel) {
                    std::string hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));
                    m_fields->m_healthLabel->setString(hp.c_str());

                    // Update health bar
                    float maxWidth = m_fields->m_healthBar->getContentWidth() - 4.f;
                    float newWidth = maxWidth * (m_fields->m_health / 100.f);

                    m_fields->m_healthBarFill->setTextureRect({ 0, 0, newWidth, 8 });
                };

                if (m_fields->m_health <= 0.f) {
                    log::warn("Player health is dead: {}", m_fields->m_health);
                    PlayLayer::destroyPlayer(player, game);
                };
            };
        };
    };
};