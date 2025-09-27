#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FreezePlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("freeze", false);
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        if (m_fields->enabled) {
            auto rnd = Rand::fast();
            if (auto gm = GameManager::sharedState()) {
                if (rnd % 100 < 10) { // 10% chance
                    capFPS(1.f);
                }
            }
        } else {
            log::warn("Random freezing at 90% is disabled");
        };

        return true;
    };

    void pauseGame(bool p0) {
        revertFPS();
        PlayLayer::pauseGame(p0);
    };

    void revertFPS() {
        // default to user old fps
        auto gm = GameManager::get();

        float oldFPS = horribleMod->getSavedValue<float>("fps");

        gm->setGameVariable("0116", true);

        // Use seconds per frame, not raw FPS
        float interval = (oldFPS > 10.f) ? (1.f / oldFPS) : (1.f / 60.f); // minimum 10 FPS
        if (interval <= 0.0f || interval > 1.0f)
            interval = 1.f / 60.f; // fallback to 60 FPS if invalid

        CCDirector::sharedDirector()->setAnimationInterval(interval);
        log::debug("reset fps to {} (interval {})", oldFPS, interval);
    };

    void capFPS(float value) {
        auto gm = GameManager::get();

        gm->setGameVariable("0116", true);
        gm->setGameVariable("0116", true);

        float interval = 1.f / value; // cap fps to 60
        if (interval <= 0.0f || interval > 1.0f)
            interval = 1.f / 60.f; // fallback to 60 FPS if invalid

        CCDirector::sharedDirector()->setAnimationInterval(interval);
        log::debug("cap fps to {} (interval {})", value, interval);
    };

    void onQuit() {
        revertFPS();
        PlayLayer::onQuit();
    }
};