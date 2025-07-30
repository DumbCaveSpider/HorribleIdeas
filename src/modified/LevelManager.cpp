#include <cocos2d.h>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace LevelManager {
    void DownloadGriefLevel() {
        auto horribleMod = getMod();
        if (horribleMod->getSavedValue<bool>("grief", false)) {
            auto glm = GameLevelManager::get();
            auto mdm = MusicDownloadManager::sharedState();
            auto level = glm->getSavedLevel(105001928);

            if (glm->hasDownloadedLevel(105001928)) {
                log::info("Grief already downloaded, skipping download");
                return;
            } else {
                glm->downloadLevel(105001928, false);
                mdm->downloadSong(482872);

                log::info("Downloading Grief in background");
            };
        };
    };

    void DownloadCongregLevel() {
        auto horribleMod = getMod();
        if (horribleMod->getSavedValue<bool>("congregation", false)) {
            auto glm = GameLevelManager::get();
            auto mdm = MusicDownloadManager::sharedState();
            auto level = glm->getSavedLevel(93437568);

            if (glm->hasDownloadedLevel(93437568)) {
                log::info("Congregation already downloaded, skipping download");
                return;
            } else {
                glm->downloadLevel(93437568, false);
                mdm->downloadSong(895761);

                log::info("Downloading Congregation in background");
            };
        };
    };
};