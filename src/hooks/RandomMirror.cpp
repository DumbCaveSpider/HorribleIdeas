#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(RandomMirrorGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool isFlipped = false;
        bool inBuffer = false;
    };

    void endFlipBuffer() {
        m_fields->inBuffer = false;
        // log::debug("flip buffer ended");
    };

    void update(float p0) {
        auto rnd = rand() % 101;
        // log::debug("gjbasegamelayer update chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("random_mirror", false)) {
            if (!m_fields->inBuffer && rnd <= static_cast<int>(horribleMod->getSettingValue<int64_t>("random_mirror-chance"))) {
                if (auto playLayer = PlayLayer::get()) {
                    // flip state
                    if (!m_fields->isFlipped) {
                        toggleFlipped(true, false); // bool 1 = flip, bool 2 = instant
                        log::debug("flipped");

                        m_fields->isFlipped = true;
                    } else {
                        toggleFlipped(false, false);
                        log::debug("unflipped");

                        m_fields->isFlipped = false;
                    };

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

    void toggleFlipped(bool p0, bool p1) {
        GJBaseGameLayer::toggleFlipped(p0, p1);
    };
};