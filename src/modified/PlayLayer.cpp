#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayLayer.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;

class $modify(HorriblePlayLayer, PlayLayer) {
    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("achieve", false)) {
            if (auto fmod = FMODAudioEngine::sharedEngine()) {
                fmod->playEffectAsync("achievement_01.ogg");
            };
        } else {
            log::warn("Random achievements is disabled");
        };

        if (horribleMod->getSavedValue<bool>("oxygen", false)) {
            log::info("oxygen level time!");
        } else {
            log::warn("Oxygen in water levels is disabled");
        };

        if (horribleMod->getSavedValue<bool>("freeze", false)) {
            if (auto gm = GameManager::sharedState()) {
                // gm or ccdir dont have native set max fps methods
            };
        } else {
            log::warn("Random freezing at 90% is disabled");
        };

        return true;
    };

    void onExit() {
        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("achieve", false)) {
            if (auto fmod = FMODAudioEngine::sharedEngine()) {
                fmod->playEffectAsync("achievement_01.ogg");
            };
        } else {
            log::warn("Random achievements is disabled");
        };

        PlayLayer::onExit();
    };
};