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
        if (m_fields->enabled) scheduleOnce(schedule_selector(RandomMirrorPlayLayer::flipPortal), static_cast<float>(randng::get(15, 3)) * chanceToDelayPct(m_fields->chance));
        PlayLayer::setupHasCompleted();
    };

    void toggleFlipped(bool p0, bool p1) {
        m_fields->isFlipped = p0;
        log::debug("{}", m_fields->isFlipped ? "flipped" : "unflipped");

        PlayLayer::toggleFlipped(p0, p1);
    };

    void nextFlipPortal() {
        if (m_fields->enabled) scheduleOnce(schedule_selector(RandomMirrorPlayLayer::flipPortal), static_cast<float>(randng::get(10, 1)) * chanceToDelayPct(m_fields->chance));
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