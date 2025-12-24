#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(PausePlayerObject, PlayLayer) {
    struct Fields {
        bool enabled = options::get("pauses");
        int chance = options::getChance("pauses");
    };

    void setupHasCompleted() {
        if (m_fields->enabled) nextPause();
        PlayLayer::setupHasCompleted();
    };

    void nextPause() {
        if (m_fields->enabled) scheduleOnce(schedule_selector(PausePlayerObject::pause), randng::get(15.f, 3.f) * chanceToDelayPct(m_fields->chance));
    };

    void pause(float) {
        if (m_fields->enabled) {
            nextPause();
            pauseGame(true);
        };
    };
};