#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayLayer.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;

class $modify(HorriblePlayLayer, PlayLayer) {
    struct Fields {
        GameObject* m_destroyingObject;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("achieve", false)) {
            if (auto fmod = FMODAudioEngine::sharedEngine()) {
                // @geode-ignore(unknown-resource)
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
                // @geode-ignore(unknown-resource)
                fmod->playEffectAsync("achievement_01.ogg");
            };
        } else {
            log::warn("Random achievements is disabled");
        };

        PlayLayer::onExit();
    };

    void destroyPlayer(PlayerObject * player, GameObject * game) {
        auto horribleMod = getMod();

        if (!m_fields->m_destroyingObject) m_fields->m_destroyingObject = game;

        if (horribleMod->getSavedValue("grief", false)) {
            if (game == m_fields->m_destroyingObject) { // fake spike at beginning
                PlayLayer::destroyPlayer(player, game);
            } else if (auto glm = GameLevelManager::sharedState()) {
                auto lvlId = 105001928;
                if (m_level->m_levelID.value() == lvlId) return PlayLayer::destroyPlayer(player, game);

                glm->downloadLevel(lvlId, true);

                if (auto level = glm->getSavedLevel(lvlId)) {
                    glm->saveLevel(level);

                    if (level->m_levelNotDownloaded) {
                        log::error("good grief!");
                    } else if (auto ccdir = CCDirector::sharedDirector()) { // find a way to optimize level switch
                        ccdir->replaceScene(PlayLayer::scene(level, false, false));
                    };
                } else {
                    log::error("good grief!!!");
                    PlayLayer::destroyPlayer(player, game);
                };
            } else {
                PlayLayer::destroyPlayer(player, game);
            };
        } else {
            log::warn("Go back on Grief is disabled");
            PlayLayer::destroyPlayer(player, game);
        };
    };
};