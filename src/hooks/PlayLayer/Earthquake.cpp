#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(EarthquakeGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = horribleideas::get("earthquake");
    };

    void update(float p0) {
        if (m_fields->enabled) {
            // shake the camera randomly based on intensity
            auto rand = randng::fast();
            auto shakeX = (static_cast<float>(rand % 25)) * 2.f;
            auto shakeY = (static_cast<float>(rand % 25)) * 2.f;
            GJBaseGameLayer::shakeCamera(shakeX, shakeY, 0.1f);
        };
        GJBaseGameLayer::update(p0);
    };
};