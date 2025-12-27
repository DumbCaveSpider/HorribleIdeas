#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(StickyPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("sticky");
        int chance = options::getChance("sticky");

        float m_defSpeed = 0.f;

        Ref<CCLabelBMFont> m_clickLabel = nullptr;

        bool m_onGround = true;
    };

    bool init(int player, int ship, GJBaseGameLayer * gameLayer, CCLayer * layer, bool playLayer) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) return false;

        if (m_fields->enabled) {
            m_fields->enabled = playLayer;

            if (auto pl = PlayLayer::get()) {
                m_fields->m_clickLabel = CCLabelBMFont::create("Press again to un-stick!", "bigFont.fnt", pl->getScaledContentWidth() - 12.5f);
                m_fields->m_clickLabel->setID("sticky-alert"_spr);
                m_fields->m_clickLabel->setScale(0.5f);
                m_fields->m_clickLabel->setAnchorPoint({ 0.5, 0 });
                m_fields->m_clickLabel->setPosition({ getScaledContentWidth() / 2.f, 25.f });
                m_fields->m_clickLabel->setVisible(false);

                auto seq = CCSequence::create(
                    CCCallFunc::create(this, callfunc_selector(StickyPlayerObject::stickyCol1)),
                    CCDelayTime::create(0.125f),
                    CCCallFunc::create(this, callfunc_selector(StickyPlayerObject::stickyCol1)),
                    CCDelayTime::create(0.125f),
                    nullptr
                );

                pl->addChild(m_fields->m_clickLabel, 9);
                m_fields->m_clickLabel->runAction(CCRepeatForever::create(seq));
            };

            m_fields->m_onGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
        };

        return true;
    };

    void stickyCol1() {
        if (m_fields->m_clickLabel) m_fields->m_clickLabel->setColor({ 250, 250, 25 });
    };

    void stickyCol2() {
        if (m_fields->m_clickLabel) m_fields->m_clickLabel->setColor({ 255, 255, 255 });
    };

    void hitGround(GameObject * object, bool notFlipped) {
        PlayerObject::hitGround(object, notFlipped);

        if (m_fields->enabled && m_hasEverJumped) {
            if (!m_fields->m_onGround) m_fields->m_onGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;

            if (m_playerSpeed > 0.f) {
                m_fields->m_defSpeed = m_playerSpeed;

                if (m_fields->chance >= randng::fast() && m_fields->m_onGround) {
                    m_playerSpeed = 0.f;
                    if (m_fields->m_clickLabel) m_fields->m_clickLabel->setVisible(true);
                };
            };
        };
    };

    bool pushButton(PlayerButton button) {
        if (!PlayerObject::pushButton(button)) return false;

        if (m_fields->enabled) {
            if (m_fields->m_onGround) {
                m_playerSpeed = m_fields->m_defSpeed;
                if (m_fields->m_clickLabel) m_fields->m_clickLabel->setVisible(false);
            };

            m_fields->m_onGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
        };

        return true;
    };
};