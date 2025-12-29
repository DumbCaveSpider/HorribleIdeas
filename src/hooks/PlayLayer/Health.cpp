#include <Utils.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace horrible;

class $modify(HealthPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("health");

        float m_health = 100.f;

        Ref<ProgressBar> m_healthBar = nullptr;
        CCLabelBMFont* m_healthLabel = nullptr;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (m_fields->enabled) {
            m_fields->m_health = 100.f;

            if (!m_fields->m_healthBar) {
                m_fields->m_healthBar = ProgressBar::create();
                m_fields->m_healthBar->setID("health"_spr);
                m_fields->m_healthBar->setFillColor(colors::red);
                m_fields->m_healthBar->setAnchorPoint({ 0.5f, 0.5f });
                m_fields->m_healthBar->setPosition({ 10.f, getScaledContentHeight() / 2.f });
                m_fields->m_healthBar->setRotation(-90.f);

                addChild(m_fields->m_healthBar, 99);
            };

            m_fields->m_healthBar->updateProgress(m_fields->m_health);

            auto const hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));
            if (!m_fields->m_healthLabel) {
                m_fields->m_healthLabel = CCLabelBMFont::create(hp.data(), "bigFont.fnt");
                m_fields->m_healthLabel->setColor(colors::red);
                m_fields->m_healthLabel->setAnchorPoint({ 0.f, 1.f });
                m_fields->m_healthLabel->setPosition({ 2.f, (getScaledContentHeight() / 2.f) - (m_fields->m_healthBar->getScaledContentWidth() / 2.f) - 1.25f });
                m_fields->m_healthLabel->setScale(0.25f);

                addChild(m_fields->m_healthLabel, 100);
            } else {
                m_fields->m_healthLabel->setString(hp.data());
            };
        };

        return true;
    };

    void resetHealth() {
        m_fields->m_health = 100.f;

        if (m_fields->m_healthLabel) {
            auto const hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));
            m_fields->m_healthLabel->setString(hp.data());
        };

        if (m_fields->m_healthBar) m_fields->m_healthBar->updateProgress(m_fields->m_health);
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

            int rnd = randng::fast();
            if (m_fields->m_health > 0) {
                m_fields->m_health -= 0.1f;

                // log::debug("Player health is {}", m_fields->m_health);

                m_player1->playSpawnEffect();
                m_player2->playSpawnEffect();

                shakeCamera(1.25f, 2.5f, 0.00875F);

                if (m_fields->m_healthLabel) {
                    auto const hp = fmt::format("HP\n{}%", static_cast<int>(m_fields->m_health));
                    m_fields->m_healthLabel->setString(hp.data());

                    m_fields->m_healthBar->updateProgress(m_fields->m_health);
                };

                if (m_fields->m_health <= 0.f) {
                    log::warn("Player health is dead: {}", m_fields->m_health);
                    PlayLayer::destroyPlayer(player, game);
                };
            };
        };
    };
};