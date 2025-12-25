#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(RandomMirrorPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("random_mirror");
        int chance = options::getChance("random_mirror");

        bool isFlipped = false;
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();
        if (m_fields->enabled) scheduleOnce(schedule_selector(RandomMirrorPlayLayer::flipPortal), randng::get(15.f, 3.f) * chanceToDelayPct(m_fields->chance));
    };

    void toggleFlipped(bool p0, bool p1) {
        m_fields->isFlipped = p0;
        log::debug("{}", m_fields->isFlipped ? "flipped" : "unflipped");

        PlayLayer::toggleFlipped(p0, p1);
    };

    void nextFlipPortal() {
        log::debug("scheduling flip");
        if (m_fields->enabled) scheduleOnce(schedule_selector(RandomMirrorPlayLayer::flipPortal), randng::get(10.f, 1.f) * chanceToDelayPct(m_fields->chance));
    };

    void flipPortal(float) {
        if (m_fields->enabled) {
            toggleFlipped(!m_fields->isFlipped, false);

            runAction(CCSequence::createWithTwoActions(
                CCDelayTime::create(2.5f),
                CCCallFunc::create(this, callfunc_selector(RandomMirrorPlayLayer::nextFlipPortal))
            ));
        };
    };
};