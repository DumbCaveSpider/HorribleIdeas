#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(RandomMirrorGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = options::get("random_mirror");
        int chance = options::getChance("random_mirror");

        bool isFlipped = false;
        bool inBuffer = false;
    };

    void endFlipBuffer() {
        m_fields->inBuffer = false;
        // log::debug("flip buffer ended");
    };

    void toggleFlipped(bool p0, bool p1) {
        m_fields->isFlipped = p0;
        log::debug("{}", m_fields->isFlipped ? "flipped" : "unflipped");

        GJBaseGameLayer::toggleFlipped(p0, p1);
    };

    void update(float p0) {
        auto rnd = randng::tiny();
        // log::debug("gjbasegamelayer update chance {}", rnd);

        if (m_fields->enabled) {
            if (!m_fields->inBuffer && rnd <= m_fields->chance) {
                if (auto playLayer = PlayLayer::get()) {
                    toggleFlipped(!m_fields->isFlipped, false);

                    // Start 5s buffer to prevent immediate re-flip
                    m_fields->inBuffer = true;

                    auto seq = CCSequence::create(
                        CCDelayTime::create(2.5f),
                        CCCallFunc::create(this, callfunc_selector(RandomMirrorGJBaseGameLayer::endFlipBuffer)),
                        nullptr
                    );

                    runAction(seq);
                };
            };
        };

        GJBaseGameLayer::update(p0);
    };
};