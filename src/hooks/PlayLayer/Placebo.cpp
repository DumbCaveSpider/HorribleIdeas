#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

struct Fields {
    bool enabled = options::get("placebo");
};

Fields m_fields;

void placeboEffect() {
    log::info("Checking for placebo effect...");

    if (m_fields.enabled) {
        int rnd = randng::fast();
        log::info("placebo effect roll: {}", rnd);

        if (rnd <= 1) { // 1% chance :trol:
            log::info("Placebo effect activated! Disabling all horrible mod options...");

            for (const auto& option : options::getAll()) {
                auto toggle = options::get(option.id);
                log::debug("Placebo {} option {}", toggle ? "disabled" : "enabled", option.id);
                options::set(option.id, !toggle);
            };
        };
    };
};

class $modify(PlaceboLevelPage, LevelPage) {
    void onPlay(CCObject * sender) {
        placeboEffect();
        log::debug("Placebo onPlay triggered");
        LevelPage::onPlay(sender);
    };
};

class $modify(PlaceboLevelInfoLayer, LevelInfoLayer) {
    void onPlay(CCObject * sender) {
        placeboEffect();
        log::debug("Placebo onPlay triggered");
        LevelInfoLayer::onPlay(sender);
    };
};