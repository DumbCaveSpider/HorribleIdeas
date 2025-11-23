#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/CCScene.hpp>
#include <Geode/modify/CCMenuItem.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;
using namespace horrible;

$execute{
    if (auto optionManager = OptionManager::get()) {
        log::debug("Registering default options...");

        for (const auto& option : allOptions) {
            optionManager->registerOption(option);
        };

        log::info("Done registering {} options", allOptions.size());
    } else {
        log::error("Failed to get OptionManager!");
    };
};

class $modify(HorribleCCScene, CCScene) {
    bool init() {
        if (!CCScene::init()) return false;

#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS)
        if (typeinfo_cast<CCTransitionFade*>(this)) {
            log::debug("scene is a CCTransitionFade");
            return true;
        };
#endif

        log::debug("scene init called");

        return true;
    };
};

// modify CCMenuItem so it plays the sound whenever a button is clicked regardless of the layer
class $modify(HorribleCCMenuItem, CCMenuItem) {
    void activate() {
        auto rnd = randng::fast();
        log::debug("button menu chance {}", rnd);

        if (horribleMod && horribleMod->getSavedValue<bool>("achieve", true)) {
            if (auto fmod = FMODAudioEngine::sharedEngine()) {

                // @geode-ignore(unknown-resource)
                if (rnd <= static_cast<int>(horribleMod->getSettingValue<int64_t>("achieve-chance"))) fmod->playEffect("achievement_01.ogg");
            };
        };

        CCMenuItem::activate();
    };
};

// safe mode
class $modify(HorribleGJGameLevel, GJGameLevel) {
    void savePercentage(int percent, bool isPracticeMode, int clicks, int attempts, bool isChkValid) {
        if (horribleMod->getSettingValue<bool>("safe-mode")) {
            log::warn("Safe mode is enabled, your progress will not be saved!");
        } else {
            GJGameLevel::savePercentage(percent, isPracticeMode, clicks, attempts, isChkValid);
        };
    };
};

// WOMP WOMP BROKEN PLAYTEST
class $modify(HorribleLevelEditorLayer, LevelEditorLayer) {
    void onPlaytest() {
        FLAlertLayer::create(
            "Warning",
            "<cy>Horrible Ideas</c> has <cr>completely broken</c> the Playtest function which prevents the player from testing the level... Sorry about that!\n<cg>We recommend disabling the mod while editing levels.</c>",
            "OK")
            ->show();
        LevelEditorLayer::onPlaytest();
    };
};