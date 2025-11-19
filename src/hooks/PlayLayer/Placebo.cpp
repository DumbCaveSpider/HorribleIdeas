#include <Horrible.hpp>

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

    if (horribleMod && m_fields.enabled) {
        int rnd = randng::tiny() % 1000;
        log::info("placebo effect roll: {}", rnd);

        if (rnd <= 10) { // 1% chance :trol:
            log::info("Placebo effect activated! Disabling all horrible mod options...");

            for (const auto& option : options::getAll()) {
                // if the options is disabled, enabled it
                if (options::get(option.id) == false) {
                    log::debug("Enabled option: {}", option.id);
                    options::set(option.id, true);
                } else {
                    log::debug("Disabled option: {}", option.id);
                    options::set(option.id, false);
                };
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