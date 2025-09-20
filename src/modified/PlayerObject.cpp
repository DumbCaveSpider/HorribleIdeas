#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

static RandomSeeder _randomSeeder;

class $modify(HorriblePlayerObject, PlayerObject) {
    struct Fields {
        int m_jumps = 0;
        bool m_isOnSlope = false;
        bool m_wasOnSlope = false;
    };

    void updateJump(float p0) {

        auto rnd = rand() % 101;
        // log::debug("player object chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("gravity", false)) {
            float newGrav = std::round((static_cast<float>(rnd) / 100.f) * (2.5f - 0.01f) * 100.0f) / 100.0f;
            auto onGrnd = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
            // Only set gravity if on flat ground (not on a slope) and not rotating
            if (onGrnd && !m_isRotating && !m_fields->m_isOnSlope && !m_fields->m_wasOnSlope) {
                m_gravityMod = newGrav;
                log::debug("set gravity to x{} (flat ground)", newGrav);
            }
        }

        PlayerObject::updateJump(p0);
    };

    void update(float p0) {

        auto healthEnabled = horribleMod->getSavedValue<bool>("health", false);

        if (auto playLayer = PlayLayer::get()) {
            // pause logic
            if (horribleMod->getSavedValue<bool>("pause", false)) {
                auto rnd = rand() % 101;
                if (rnd <= static_cast<int>(horribleMod->getSettingValue<int64_t>("pause-chance"))) {
                    // pause the game randomly
                    log::debug("Pausing the game randomly");
                    playLayer->pauseGame(true);
                };
            };

            // sleepy player
            if (horribleMod->getSavedValue<bool>("sleepy", false)) {
                log::debug("Player is sleepy!");
            };

            // lil hack on getting the percentage lmao
            auto currentPercentage = playLayer->getCurrentPercentInt();

            if (horribleMod->getSavedValue<bool>("freeze", false)) {
                // check if current percentage is less than or equal to 90
                if (currentPercentage >= 90) {
                    auto gm = GameManager::get();

                    float oldFPS = Mod::get()->getSavedValue<float>("fps");

                    gm->setGameVariable("0116", true);

                    // Randomize FPS between 1 and 30
                    int randomFPS = 1 + (rand() % 31); // 1 to 30 inclusive

                    float interval = 1.f / static_cast<float>(randomFPS);
                    if (interval <= 0.0f || interval > 1.0f)
                        interval = 1.f / 60.f; // fallback to 60 FPS if invalid

                    CCDirector::sharedDirector()->setAnimationInterval(interval);

                    // log::debug("cap fps to {} (interval {})", randomFPS, interval);
                } else {
                    // default to user old fps
                    auto gm = GameManager::get();

                    float oldFPS = Mod::get()->getSavedValue<float>("fps");

                    gm->setGameVariable("0116", true);

                    // Use seconds per frame, not raw FPS
                    float interval = (oldFPS > 10.f) ? (1.f / oldFPS) : (1.f / 60.f); // minimum 10 FPS
                    if (interval <= 0.0f || interval > 1.0f)
                        interval = 1.f / 60.f; // fallback to 60 FPS if invalid

                    CCDirector::sharedDirector()->setAnimationInterval(interval);

                    // log::debug("reset fps to {} (interval {})", oldFPS, interval);
                };
            };
        };

        PlayerObject::update(p0);
    };
};
