#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FlashbangPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("flashbang", false);
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;
        if (m_fields->enabled) {
            log::debug("flashbang enabled, init scheduling flashbang");
            int rnd = randng::fast();

            // random delay between 1 and 10 seconds
            float delay = static_cast<float>(rnd % 10);
            log::debug("Flashbang will appear after {} seconds", delay);

            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(FlashbangPlayLayer::showFlashbang),
                this, delay, false);

        }
        return true;
    };

    void showFlashbang(float) {
        if (m_fields->enabled) {
            log::debug("think fast chucklenuts, flashbang incoming");
            auto fmod = FMODAudioEngine::sharedEngine();
            fmod->playEffect("Flashbang.mp3"_spr);

            auto winSize = CCDirector::sharedDirector()->getWinSize();

            auto flashbang = CCScale9Sprite::create("square02b_001.png");
            flashbang->setContentSize({ winSize.width + 10.f, winSize.height + 10.f });
            flashbang->setPosition({ winSize.width / 2.f, winSize.height / 2.f });
            flashbang->setID("flashbang");
            flashbang->setZOrder(100);

            addChild(flashbang);

            // fade out the flashbang after 1 seconds
            flashbang->runAction(
                CCSequence::create(
                    CCDelayTime::create(3.f),
                    CCFadeOut::create(1.f),
                    CCCallFunc::create(this, callfunc_selector(FlashbangPlayLayer::removeFlashbang)),
                    nullptr));
        }
    };

    void removeFlashbang() {
        if (auto flashbang = getChildByID("flashbang")) {
            flashbang->removeFromParent();
        }
        float delay = randng::fast() % 20 + 10; // between 10 and 30 seconds
        log::debug("Next flashbang will appear after {} seconds", delay);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
            schedule_selector(FlashbangPlayLayer::showFlashbang),
            this, delay, false);
    }
};