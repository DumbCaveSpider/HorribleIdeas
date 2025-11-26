#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(GamblerPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("gambler");

        bool triggered = false;
        bool tricked = false;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (m_fields->enabled) {
            log::debug("gambler enabled");

            // check every frame so we can detect each percentage change
            if (!m_fields->triggered) {
                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                    schedule_selector(GamblerPlayLayer::gamblerCheck),
                    this, 0.f, false);
            };
        };

        return true;
    };

    // ensure that triggered is reset on level restart/full reset
    void fullReset() {
        m_fields->triggered = false;
        log::debug("gambler full reset");
        PlayLayer::fullReset();
    };

    void resetLevel() {
        m_fields->triggered = false;
        log::debug("gambler level reset");
        PlayLayer::resetLevel();
    };

    void gamblerCheck(float) {
        if (!m_fields->enabled) {
            int percentage = getCurrentPercentInt();
            // detect the moment the player first reaches or crosses 95
            if (percentage == 95 && !m_fields->triggered) {
                // roll a random number between 0 and 1
                int roll = randng::fast() % 2;

                log::info("Gambler roll: {}", roll);
                if (roll == 0) {
                    log::info("Gambler lost the bet!");

                    // reverse the player
                    m_player1->reversePlayer(nullptr);
                    m_player1->m_gravity = 0.01f; // reduce gravity to simulate a bounce

                    // force player to jump
                    GJBaseGameLayer::get()->handleButton(true, 1, true);

                    m_fields->triggered = true;
                    m_fields->tricked = true;
                } else {
                    log::info("Gambler won the bet! instant win.");
                    levelComplete();
                    m_fields->triggered = true;
                };
            };
        };
    };
};