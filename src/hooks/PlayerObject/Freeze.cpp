#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FreezePlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("freeze");
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                // lil hack on getting the percentage lmao
                auto currentPercentage = playLayer->getCurrentPercentInt();

                // check if current percentage is less than or equal to 90
                if (currentPercentage >= 90) {
                    auto gm = GameManager::get();

                    float oldFPS = horribleMod->getSavedValue<float>("fps");

                    gm->setGameVariable("0116", true);

                    // Randomize FPS between 1 and 45
                    int randomFPS = 1 + (rand() % 46); // 1 to 45 inclusive

                    float interval = 1.f / static_cast<float>(randomFPS);
                    if (interval <= 0.0f || interval > 1.0f) interval = 1.f / 60.f; // fallback to 60 FPS if invalid

                    CCDirector::sharedDirector()->setAnimationInterval(interval);

                    // log::debug("cap fps to {} (interval {})", randomFPS, interval);
                } else {
                    // default to user old fps
                    auto gm = GameManager::get();

                    float oldFPS = horribleMod->getSavedValue<float>("fps");

                    gm->setGameVariable("0116", true);

                    // Use seconds per frame, not raw FPS
                    float interval = (oldFPS > 10.f) ? (1.f / oldFPS) : (1.f / 60.f); // minimum 10 FPS
                    if (interval <= 0.0f || interval > 1.0f) interval = 1.f / 60.f; // fallback to 60 FPS if invalid

                    CCDirector::sharedDirector()->setAnimationInterval(interval);

                    // log::debug("reset fps to {} (interval {})", oldFPS, interval);
                };
            };
        };

        PlayerObject::update(p0);
    };
};