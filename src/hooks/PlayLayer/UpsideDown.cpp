#include <Utils.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace horrible;

class $modify(ForceLevelsPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("upside_down");
        int chance = options::getChance("upside_down"); // chance as percent
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        if (m_fields->enabled) {
            int rnd = randng::fast(); // random float between 0 and 100
            log::debug("scene rng {} chance {}", rnd, m_fields->chance);

            if (rnd <= m_fields->chance) {
                log::debug("setting scene upside down");
                setRotation(-180.f);
            };
        };
    };
};