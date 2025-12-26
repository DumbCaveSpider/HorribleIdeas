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

    bool init(int player, int ship, GJBaseGameLayer * gameLayer, CCLayer * layer, bool playLayer) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) return false;

        if (m_fields->enabled && playLayer) scheduleOnce(schedule_selector(SleepyPlayerObject::asleep), randng::get(30.f, 5.f) * chanceToDelayPct(m_fields->chance));

        return true;
    };

    void startSleepTimer(float) {
        if (m_fields->enabled) scheduleOnce(schedule_selector(SleepyPlayerObject::wakeUpSchedule), randng::get(15.f, 3.f) * chanceToDelayPct(m_fields->chance));
    };

    void wakeUpSchedule(float) {
        log::debug("Waking the player up");

        m_fields->sleepy = false;
        m_fields->waking = true;

        m_playerSpeed = m_fields->savedDefaultSpeed; // snap back to original speed

        scheduleOnce(schedule_selector(SleepyPlayerObject::fullyWakeUpSchedule), 5.f);
    };

    // schedule wrapper for fullyWakeUp
    void fullyWakeUpSchedule(float) {
        m_fields->waking = false;
    };

    bool pushButton(PlayerButton p0) {
        // no jumping while sleepy
        if (m_fields->sleepy) return false;
        return PlayerObject::pushButton(p0);
    };

    void asleep(float) {
        if (m_fields->enabled) {
            // player sleepy if not already in any stage
            auto onGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
            if (!m_fields->sleepy && !m_fields->waking && onGround) {
                log::debug("Making the player sleepy");

                m_fields->savedDefaultSpeed = m_playerSpeed; // capture original speed
                m_fields->sleepy = true;

                scheduleOnce(schedule_selector(SleepyPlayerObject::startSleepTimer), 0.25f);

                // go to sleep, go to sleep, sweet little baby go to sleep
                schedule(schedule_selector(SleepyPlayerObject::fallAsleep), 0.125f);
            };
        } else {
            // wake up
            if (m_fields->sleepy || m_fields->waking) m_fields->waking = false;
        };
    };

    void fallAsleep(float dt) {
        // decelerate to 0 speed
        if (m_fields->sleepy) {
            auto decelRate = m_fields->savedDefaultSpeed / 5.f; // decel to 0 in 5s
            m_playerSpeed -= decelRate * dt;

            if (m_playerSpeed <= 0.f) {
                m_playerSpeed = 0.f;
                unschedule(schedule_selector(SleepyPlayerObject::fallAsleep));
            };
        };
    };
};