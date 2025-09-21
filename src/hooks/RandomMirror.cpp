#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

static RandomSeeder _randomSeeder;

class $modify(RandomMirrorGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        int chance = 0;

        bool isFlipped = false;
        bool inBuffer = false;
    };

    void endFlipBuffer() {
        m_fields->inBuffer = false;
        // log::debug("flip buffer ended");
    };

    bool init() {
        if (!GJBaseGameLayer::init()) return false;

        m_fields->chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("random_mirror-chance"));
        log::debug("Random mirror chance set to {}", m_fields->chance);

        return true;
    };

    void update(float p0) {
        auto rnd = rand() % 101;
        // log::debug("gjbasegamelayer update chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("random_mirror", false)) {
            if (!m_fields->inBuffer && rnd <= m_fields->chance) {
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