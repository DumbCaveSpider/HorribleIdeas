#include <Horrible.hpp>
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(OxygenBarPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("oxygen", false);
        bool healthEnabled = horribleMod->getSavedValue<bool>("health", false);
        CCLabelBMFont* m_oxygenLabel = nullptr;

        float m_oxygenLevel = 100.f;
        bool m_oxygenActive = false;

        Ref<CCSprite> m_oxygenBar;
        CCSprite* m_oxygenBarFill;
    };
    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        if (m_fields->enabled) {
            m_fields->m_oxygenActive = true;
            m_fields->m_oxygenLevel = 100.f;

            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(OxygenBarPlayLayer::decreaseOxygen),
                this, 0.0f, false);

            if (!m_fields->m_oxygenBar) {
                m_fields->m_oxygenBar = CCSprite::create("slidergroove2.png");
                m_fields->m_oxygenBar->setID("oxygen"_spr);
                m_fields->m_oxygenBar->setPosition({ 10.f, getScaledContentHeight() / 2.f });
                m_fields->m_oxygenBar->setRotation(90.f);
                m_fields->m_oxygenBar->setZOrder(101);

                m_fields->m_oxygenBarFill = CCSprite::create("sliderBar2.png");
                m_fields->m_oxygenBarFill->setID("oxygen-fill"_spr);
                m_fields->m_oxygenBarFill->setZOrder(-1);
                m_fields->m_oxygenBarFill->setRotation(-180.f);
                m_fields->m_oxygenBarFill->setColor({ 0, 175, 255 });
                m_fields->m_oxygenBarFill->setPosition({ m_fields->m_oxygenBar->getScaledContentWidth() - 2.f, 4.f });
                m_fields->m_oxygenBarFill->setAnchorPoint({ 0, 1 });

                m_fields->m_oxygenBarFill->setTextureRect({ 0, 0, 0, 8 });

                m_fields->m_oxygenBar->addChild(m_fields->m_oxygenBarFill);

                addChild(m_fields->m_oxygenBar);
            };

            if (m_fields->healthEnabled) {
                m_fields->m_oxygenBar->setPosition({ 30.f, getScaledContentHeight() / 2.f });
            } else {
                // center if health bar is disabled
                m_fields->m_oxygenBar->setPosition({ 10.f, getScaledContentHeight() / 2.f });
            };

            std::string buf = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));

            if (!m_fields->m_oxygenLabel) {
                m_fields->m_oxygenLabel = CCLabelBMFont::create(buf.c_str(), "bigFont.fnt");
                m_fields->m_oxygenLabel->setColor({ 0, 175, 255 });
                m_fields->m_oxygenLabel->setAnchorPoint({ 0.f, 1.0f });
                m_fields->m_oxygenLabel->setPosition({ 2.f, (getScaledContentHeight() / 2.f) - (m_fields->m_oxygenBar->getScaledContentWidth() / 2.f) - 1.25f });
                m_fields->m_oxygenLabel->setScale(0.25f);

                addChild(m_fields->m_oxygenLabel, 100);
            } else {
                m_fields->m_oxygenLabel->setString(buf.c_str());
            };

            m_fields->m_oxygenLabel->setPosition({ m_fields->m_oxygenBar->getPositionX() + 2.f - 10.f, (getScaledContentHeight() / 2.f) - (m_fields->m_oxygenBar->getScaledContentWidth() / 2.f) - 1.25f });
        } else {
            log::warn("Oxygen meter is disabled");
        };
        return true;
    };

    void decreaseOxygen(float dt) {
        if (!m_fields->m_oxygenActive || !m_fields->m_oxygenLabel)
            return;
        if (!m_player1)
            return;

        // regen o2
        if (m_player1->m_isBird || m_player1->m_isShip || m_player1->m_isSwing || m_player1->m_isDart) {
            m_fields->m_oxygenLevel += 5.f * dt;
            // log::debug("Oxygen level increased: {}", m_fields->m_oxygenLevel);
        } else {
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

        m_fields->m_oxygenBarFill->setTextureRect({ 0, 0, newWidth, 8 });

        // Destroy player if oxygen is 0
        if (m_fields->m_oxygenLevel <= 0.f && m_player1 && !m_player1->m_isDead)
            destroyPlayer(m_player1, nullptr);
    };

    void resetOxygenLevel() {
        m_fields->m_oxygenLevel = 100.f;

        if (m_fields->m_oxygenLabel) {
            std::string buf = fmt::format("o2\n{}%", static_cast<int>(m_fields->m_oxygenLevel));
            m_fields->m_oxygenLabel->setString(buf.c_str());
        };
    };

    void resetLevel() {
        resetOxygenLevel();
        PlayLayer::resetLevel();
    }
};