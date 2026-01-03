#include <Utils.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace horrible;

class $modify(OxygenPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("oxygen");

        bool withHealth = options::get("health");

        float m_oxygenLevel = 100.f;
        bool m_oxygenActive = false;

        ProgressBar* m_oxygenBar = nullptr;
        CCLabelBMFont* m_oxygenLabel = nullptr;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (m_fields->enabled) {
            m_fields->m_oxygenActive = true;
            m_fields->m_oxygenLevel = 100.f;

            schedule(schedule_selector(OxygenPlayLayer::decreaseOxygen), 0.1f);

            if (!m_fields->m_oxygenBar) {
                m_fields->m_oxygenBar = ProgressBar::create();
                m_fields->m_oxygenBar->setID("oxygen"_spr);
                m_fields->m_oxygenBar->setFillColor(colors::cyan);
                m_fields->m_oxygenBar->setAnchorPoint({ 0.5f, 0.5f });
                m_fields->m_oxygenBar->setPosition({ 10.f, getScaledContentHeight() / 2.f });
                m_fields->m_oxygenBar->setRotation(-90.f);

                m_uiLayer->addChild(m_fields->m_oxygenBar, 99);
            };

            m_fields->m_oxygenBar->updateProgress(m_fields->m_oxygenLevel);

            if (m_fields->withHealth) m_fields->m_oxygenBar->setPositionX(m_fields->m_oxygenBar->getPositionX() + 25.f);

            auto const o2 = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));
            if (!m_fields->m_oxygenLabel) {
                m_fields->m_oxygenLabel = CCLabelBMFont::create(o2.c_str(), "bigFont.fnt");
                m_fields->m_oxygenLabel->setColor(colors::cyan);
                m_fields->m_oxygenLabel->setAnchorPoint({ 0.f, 1.f });
                m_fields->m_oxygenLabel->setPosition({ 2.f, (getScaledContentHeight() / 2.f) - (m_fields->m_oxygenBar->getScaledContentWidth() / 2.f) - 1.25f });
                m_fields->m_oxygenLabel->setScale(0.25f);

                m_uiLayer->addChild(m_fields->m_oxygenLabel, 100);
            } else {
                m_fields->m_oxygenLabel->setString(o2.c_str());
            };

            m_fields->m_oxygenLabel->setPosition({ m_fields->m_oxygenBar->getPositionX() + 2.f - 10.f, (getScaledContentHeight() / 2.f) - (m_fields->m_oxygenBar->getScaledContentWidth() / 2.f) - 1.25f });
        } else {
            log::warn("Oxygen meter is disabled");
        };

        return true;
    };

    void decreaseOxygen(float dt) {
        if (!m_fields->m_oxygenActive || !m_fields->m_oxygenLabel) return;
        if (!m_player1) return;

        // regen o2
        if (m_player1->m_isBird || m_player1->m_isShip || m_player1->m_isSwing || m_player1->m_isDart) {
            m_fields->m_oxygenLevel += 5.f * dt;
            // log::debug("Oxygen level increased: {}", m_fields->m_oxygenLevel);
        } else {
            m_fields->m_oxygenLevel -= 2.f * dt;
            // log::debug("Oxygen level decreased: {}", m_fields->m_oxygenLevel);
        };

        if (m_fields->m_oxygenLevel > 100.f) m_fields->m_oxygenLevel = 100.f;
        if (m_fields->m_oxygenLevel < 0.f) m_fields->m_oxygenLevel = 0.f;

        auto const o2 = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));
        m_fields->m_oxygenLabel->setString(o2.c_str());

        m_fields->m_oxygenBar->updateProgress(m_fields->m_oxygenLevel);

        // Destroy player if oxygen is 0
        if (m_fields->m_oxygenLevel <= 0.f && m_player1 && !m_player1->m_isDead) destroyPlayer(m_player1, nullptr);
    };

    void resetOxygenLevel() {
        m_fields->m_oxygenLevel = 100.f;

        if (m_fields->m_oxygenLabel) {
            auto const o2 = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));
            m_fields->m_oxygenLabel->setString(o2.c_str());
        };
    };

    void resetLevel() {
        resetOxygenLevel();
        PlayLayer::resetLevel();
    };
};