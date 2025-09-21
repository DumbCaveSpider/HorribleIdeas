#include "../LevelManager.hpp"

#include <Horrible.hpp>

#include <cocos2d.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

void LevelManager::DownloadGriefLevel() {
    if (horribleMod->getSavedValue<bool>("grief", false)) {
        auto glm = GameLevelManager::get();

        if (glm->hasDownloadedLevel(105001928)) {
            log::info("Grief already downloaded, skipping download");
            glm->updateLevel(glm->getSavedLevel(105001928));
        } else {
            auto mdm = MusicDownloadManager::sharedState();

            log::info("Downloading Grief in background");

            glm->downloadLevel(105001928, false);
            mdm->downloadSong(482872);
        };
    };
};

void LevelManager::DownloadCongregLevel() {
    if (horribleMod->getSavedValue<bool>("congregation", false)) {
        auto glm = GameLevelManager::get();

        if (glm->hasDownloadedLevel(93437568)) {
            log::info("Congregation already downloaded, skipping download");
            glm->updateLevel(glm->getSavedLevel(93437568));
        } else {
            auto mdm = MusicDownloadManager::sharedState();

            glm->downloadLevel(93437568, false);
            mdm->downloadSong(895761);

            log::info("Downloading Congregation in background");
        };
    };
};