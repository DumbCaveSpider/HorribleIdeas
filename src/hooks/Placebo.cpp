#include <Utils.hpp>

#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace horrible;

void placeboEffect() {
    log::info("Checking for placebo effect...");

    if (options::get("placebo")) {
        int rnd = randng::fast();
        log::info("placebo effect roll: {}", rnd);

        if (rnd <= 1) { // 1% chance :trol:
            log::info("Placebo effect activated! Disabling all horrible mod options...");

            for (auto const& option : options::getAll()) {
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
        log::debug("Placebo triggered in level page");

        LevelPage::onPlay(sender);
    };
};

class $modify(PlaceboLevelInfoLayer, LevelInfoLayer) {
    void onPlay(CCObject * sender) {
        placeboEffect();
        log::debug("Placebo triggered in level info layer");

        LevelInfoLayer::onPlay(sender);
    };
};