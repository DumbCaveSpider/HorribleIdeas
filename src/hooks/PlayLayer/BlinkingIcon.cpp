#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(BlinkingIconPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("blinking_icon");
        int chance = options::getChance("blinking_icon");
    };

    void setupHasCompleted() {
        if (m_fields->enabled) nextBlink();
        PlayLayer::setupHasCompleted();
    };

    void nextBlink() {
        if (m_fields->enabled) scheduleOnce(schedule_selector(BlinkingIconPlayLayer::blink), randng::get(3.f, 1.f) * chanceToDelayPct(m_fields->chance));
    };

    void blink(float) {
        if (m_fields->enabled) {
            log::debug("Blink!");

            if (m_player1) m_player1->playSpawnEffect();
            if (m_player2) m_player2->playSpawnEffect();

            nextBlink();
        };
    };
};