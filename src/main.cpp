#include <menu/FloatingButton.hpp>

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/CCScene.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCMenuItem.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include <Geode/loader/SettingV3.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;
using namespace horrible;

$execute{
    if (auto om = OptionManager::get()) {
        log::debug("Registering default options...");

        for (const auto& option : allOptions) {
            om->registerOption(option);
        };

        log::info("Done registering {} options", allOptions.size());
    } else {
        log::error("Failed to get OptionManager!");
    };

    listenForSettingChanges("floating-button", [=](bool value) {
        if (auto fb = FloatingButton::get()) fb->setVisible(value);
                            });

    listenForSettingChanges("floating-button-level", [=](bool value) {
        if (auto fb = FloatingButton::get()) fb->setShowInLevel(value);
                            });

    listenForSettingChanges("floating-button-scale", [=](double value) {
        if (auto fb = FloatingButton::get()) fb->setScale(static_cast<float>(value));
                            });

    listenForSettingChanges("floating-button-opacity", [=](int64_t value) {
        if (auto fb = FloatingButton::get()) fb->setOpacity(value);
                            });
};

// class $modify(HICCScene, CCScene) {
//       bool init() {
//             if (!CCScene::init()) return false;

// #if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS)
//             if (typeinfo_cast<CCTransitionFade*>(this)) {
//                   log::debug("scene is a CCTransitionFade");
//                   return true;
//             };
// #endif

//             log::debug("scene init called");

//             return true;
//       };
// };

class $modify(HIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (auto fb = FloatingButton::get()) SceneManager::get()->keepAcrossScenes(fb);

        return true;
    };
};

// modify CCMenuItem so it plays the sound whenever a button is clicked regardless of the layer
class $modify(HICCMenuItem, CCMenuItem) {
    void activate() {
        CCMenuItem::activate();

        int rnd = randng::fast();
        log::debug("button menu chance {}", rnd);

        if (options::get("achieve")) {
            if (auto fmod = FMODAudioEngine::sharedEngine()) {
                // @geode-ignore(unknown-resource)
                if (rnd <= options::getChance("achieve")) fmod->playEffectAsync("achievement_01.ogg");
            };
        };
    };
};

// safe mode
class $modify(HIGJGameLevel, GJGameLevel) {
    void savePercentage(int percent, bool isPracticeMode, int clicks, int attempts, bool isChkValid) {
        if (horribleMod->getSettingValue<bool>("safe-mode")) {
            log::warn("Safe mode is enabled, your progress will not be saved!");
        } else {
            GJGameLevel::savePercentage(percent, isPracticeMode, clicks, attempts, isChkValid);
        };
    };
};

// WOMP WOMP BROKEN PLAYTEST
class $modify(HILevelEditorLayer, LevelEditorLayer) {
    void onPlaytest() {
        FLAlertLayer::create(
            "Warning",
            "<cy>Horrible Ideas</c> has somehow <cr>completely broke</c> the Playtest function in the level editor which now prevents the player from playtesting the level... Sorry about that!\n\n<cg>We recommend disabling the mod while working on levels.</c>",
            "OK")
            ->show();

        LevelEditorLayer::onPlaytest();
    };
};