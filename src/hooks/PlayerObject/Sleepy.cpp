#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(SleepyPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("sleepy");
        int chance = options::getChance("sleepy");

        bool sleepy = false; // decelerating-to-zero stage
        bool waking = false; // 5s buffer stage (cannot be re-slept)

        float savedDefaultSpeed = 0.f; // original speed captured at sleep start
    };

    void startSleepTimer() {
        // begin waking stage
        auto seq = CCSequence::create(
            CCDelayTime::create(5.f),
            CCCallFunc::create(this, callfunc_selector(SleepyPlayerObject::wakeUp)),
            nullptr);

        runAction(seq);
    };

    void wakeUp() {
        log::debug("Waking the player up");

        m_fields->sleepy = false;
        m_fields->waking = true;

        m_playerSpeed = m_fields->savedDefaultSpeed; // snap back to original speed

        // 5 seconds buffer before fully awake
        auto buffer = CCSequence::createWithTwoActions(
            CCDelayTime::create(5.f),
            CCCallFunc::create(this, callfunc_selector(SleepyPlayerObject::fullyWakeUp))
        );

        runAction(buffer);
    };

    void fullyWakeUp() {
        m_fields->waking = false;
    };

    bool pushButton(PlayerButton p0) {
        // no jumping while sleepy
        if (m_fields->sleepy) return false;

        PlayerObject::pushButton(p0);
        return true;
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                // player sleepy if not already in any stage
                auto onGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
                if (!m_fields->sleepy && !m_fields->waking && onGround) {
                    int rnd = randng::tiny();

                    // if the rng is lower than the chance, make the player sleepy
                    if (rnd <= m_fields->chance) {
                        log::debug("Making the player sleepy");

                        m_fields->savedDefaultSpeed = m_playerSpeed; // capture original speed
                        m_fields->sleepy = true;

                        startSleepTimer();
                    };
                };

                // go to sleep, go to sleep, sweet little baby go to sleep
                if (m_fields->sleepy) {
                    m_playerSpeed *= 0.99f;
                    if (m_playerSpeed < 0.1f) m_playerSpeed = 0.f;
                };
            } else {
                // wake up
                if (m_fields->sleepy || m_fields->waking) fullyWakeUp();
            };

            PlayerObject::update(p0);
        };
    };
};
