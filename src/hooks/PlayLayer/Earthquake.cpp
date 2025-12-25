#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(EarthquakePlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("earthquake");
    };

    void setupHasCompleted() {
        if (m_fields->enabled) nextQuake();
        PlayLayer::setupHasCompleted();
    };

    void nextQuake() {
        auto delay = randng::get(3.f, 1.f);
        log::debug("scheduling quake in {}s", delay);

        if (m_fields->enabled) scheduleOnce(schedule_selector(EarthquakePlayLayer::quake), delay);
    };

    void quake(float) {
        if (m_fields->enabled) {
            // shake the camera randomly based on intensity
            int rnd = randng::fast();

            auto shakeX = (static_cast<float>(rnd % 25)) * 1.25f;
            auto shakeY = (static_cast<float>(rnd % 25)) * 1.25f;

            shakeCamera(shakeX, shakeY, 0.1f);
        };

        nextQuake();
    };
};