#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FriendsPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("friends");

        std::vector<std::string> friends = {
            "diffIcon_00_btn_001.png",
            "diffIcon_01_btn_001.png",
            "diffIcon_02_btn_001.png",
            "diffIcon_03_btn_001.png",
            "diffIcon_04_btn_001.png",
            "diffIcon_05_btn_001.png",
            "diffIcon_06_btn_001.png",
            "diffIcon_07_btn_001.png",
            "diffIcon_08_btn_001.png",
            "diffIcon_09_btn_001.png",
            "diffIcon_10_btn_001.png",
            "diffIcon_auto_btn_001.png"
        };
    };

    void setupHasCompleted() {
        if (m_fields->enabled) {
            int rnd = randng::tiny();
            log::info("playlayer setup completed {}", rnd);

            // random delay between 0 and 10 seconds
            float delay = static_cast<float>(rnd % 10001) / 1000.f;
            log::debug("Friend will visit after {} seconds", delay);

            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(FriendsPlayLayer::showAFriend),
                this, delay, false);
        };

        PlayLayer::setupHasCompleted();
    };

    void showAFriend(float) {
        if (m_fields->enabled) {
            auto rnd = randng::fast();

            float xA = -125.f; // starting x pos
            float xB = getScaledContentWidth() + 125.f; // ending x pos

            if ((rnd / 2.5) <= 37.5) { xA = xB; xB = -125.f; }; // swap sides

            auto rA = static_cast<float>(randng::fast()) / 100.f;
            auto rB = static_cast<float>(randng::fast()) / 100.f;

            float yA = getScaledContentHeight() * rA; // starting height pos
            float yB = getScaledContentHeight() * rB; // ending height pos

            auto idx = randng::get(m_fields->friends.size() - 1);
            auto friendSprite = CCSprite::createWithSpriteFrameName(m_fields->friends[idx].c_str());
            friendSprite->setID("friend"_spr);
            friendSprite->setPosition({ xA, yA });
            friendSprite->setScale(1.25 * (rB + rA));
            friendSprite->setRotation(360.f * (yA * yB)); // random rotation

            addChild(friendSprite);

            auto move = CCMoveTo::create(12.5f * rA, { xB, yB });
            auto rotate = CCRotateBy::create(12.5f * rB, 90.f * (rB + rA)); // slight rotation while moving

            auto action = CCSpawn::createWithTwoActions(move, rotate);
            auto finish = CCCallFunc::create(this, callfunc_selector(FriendsPlayLayer::cleanupFriend));
            friendSprite->runAction(CCSequence::createWithTwoActions(action, finish));
        };
    };

    void cleanupFriend() {
        if (auto friendSprite = getChildByID("friend"_spr)) {
            friendSprite->removeMeAndCleanup();
            log::debug("Friend has exited");

            if (m_fields->enabled) {
                int rnd = randng::tiny();

                float delay = (static_cast<float>(rnd % 87501) / 1000.f) + 1.25f; // random delay between 1.25 and 10 seconds
                log::debug("Friend will visit again after {} seconds", delay);

                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                    schedule_selector(FriendsPlayLayer::showAFriend),
                    this, delay, false);
            };
        };
    };
};