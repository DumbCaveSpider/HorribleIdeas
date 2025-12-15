#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FriendsPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("friends");

        std::vector<std::string> const friends = {
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
            int rnd = randng::get(10);
            log::info("playlayer setup completed {}", rnd);

            float delay = static_cast<float>(rnd);
            log::debug("Friend will visit after {} seconds", delay);

            scheduleOnce(schedule_selector(FriendsPlayLayer::showAFriend), delay);
        };

        PlayLayer::setupHasCompleted();
    };

    void showAFriend(float) {
        if (m_fields->enabled) {
            int rnd = randng::fast();

            float xA = -125.f; // starting x pos
            float xB = getScaledContentWidth() + 125.f; // ending x pos

            if ((rnd / 2) <= 50.0) { xA = xB; xB = -125.f; }; // swap sides

            auto rA = static_cast<float>(randng::fast()) / 100.f;
            auto rB = static_cast<float>(randng::fast()) / 100.f;

            float yA = getScaledContentHeight() * rA; // starting height pos
            float yB = getScaledContentHeight() * rB; // ending height pos

            auto idx = randng::get(m_fields->friends.size() - 1);
            Ref<CCSprite> friendSpr = CCSprite::createWithSpriteFrameName(m_fields->friends[idx].c_str());
            friendSpr->setID("friend"_spr);
            friendSpr->setPosition({ xA, yA });
            friendSpr->setScale(1.25 * (rB + rA));
            friendSpr->setRotation(180.f * (yA * yB)); // random rotation
            friendSpr->setZOrder(9);

            auto dur = 12.5f * rA;
            auto move = CCMoveTo::create(dur, { xB, yB });
            auto rotate = CCRotateBy::create(dur, 90.f * (rB + rA)); // slight rotation while moving

            auto action = CCSpawn::createWithTwoActions(move, rotate);
            auto finish = CCCallFuncN::create(this, callfuncN_selector(FriendsPlayLayer::cleanupFriend));

            auto friendAction = CCSequence::createWithTwoActions(action, finish);
            auto scheduleAction = CCSpawn::create(CCCallFunc::create(this, callfunc_selector(FriendsPlayLayer::scheduleNextFriend)), nullptr);

            addChild(friendSpr);
            friendSpr->runAction(CCSpawn::createWithTwoActions(friendAction, scheduleAction));
        };
    };

    void cleanupFriend(CCNode * sender) {
        if (sender) sender->removeMeAndCleanup();
    };

    void scheduleNextFriend() {
        if (m_fields->enabled) {
            float delay = static_cast<float>(randng::get(5));
            log::debug("Friend will visit again after {} seconds", delay);

            scheduleOnce(schedule_selector(FriendsPlayLayer::showAFriend), delay);
        };
    };
};