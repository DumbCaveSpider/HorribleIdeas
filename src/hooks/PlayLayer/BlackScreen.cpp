#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(BlackScreenPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("black_screen");
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        int rnd = randng::tiny();
        log::info("playlayer init called {}", rnd);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        if (horribleMod && m_fields->enabled) {
            log::debug("black screen enabled, init scheduling black screen");

            // random delay between 0 and 10 seconds
            float delay = static_cast<float>(rnd % 10001) / 1000.0f;
            log::debug("Black screen will appear after {} seconds", delay);

            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(BlackScreenPlayLayer::showBlackScreen),
                this, delay, false);
        };

        return true;
    };

    void showBlackScreen(float) {
        if (m_fields->enabled) {
            log::debug("Showing black screen after delay");

            auto winSize = CCDirector::sharedDirector()->getWinSize();

            auto blackScreen = CCScale9Sprite::create("square02_001.png");
            blackScreen->setContentSize({ winSize.width + 10.f, winSize.height + 10.f });
            blackScreen->setPosition({ winSize.width / 2.f, winSize.height / 2.f });
            blackScreen->setID("black_screen"_spr);
            blackScreen->setZOrder(100);

            addChild(blackScreen);

            // Schedule removal after 0.5 seconds, then schedule to show again after a random delay
            blackScreen->runAction(
                CCSequence::create(
                    CCDelayTime::create(0.25f),
                    CCCallFunc::create(this, callfunc_selector(BlackScreenPlayLayer::removeBlackScreen)),
                    nullptr));
        };
    };

    void removeBlackScreen() {
        if (auto blackScreen = getChildByID("black_screen"_spr)) {
            blackScreen->removeFromParent();
            log::debug("Black screen removed");
            int rnd = randng::tiny();

            if (m_fields->enabled) {
                float delay = static_cast<float>(rnd % 10001) / 1000.0f; // random delay between 0 and 10 seconds
                log::debug("Black screen will appear again after {} seconds", delay);

                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                    schedule_selector(BlackScreenPlayLayer::showBlackScreen),
                    this, delay, false);
            };
        };
    };
};
