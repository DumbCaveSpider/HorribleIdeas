#include <cocos2d.h>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace LevelManager
{

    void checkAndDownloadGriefLevel()
    {
        auto horribleMod = getMod();
        if (horribleMod->getSavedValue<bool>("grief", false))
        {
            auto glm = GameLevelManager::get();
            auto level = glm->getSavedLevel(105001928);
            if (level == nullptr) {
                // Level not in saved levels, download in background
                glm->downloadLevel(105001928, false);
                log::info("Grief level not found in saved levels, downloading in background");
            } else {
                log::info("Grief level already present in saved levels");
            }
        }
    }
}