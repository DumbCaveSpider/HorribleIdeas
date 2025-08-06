#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;

class $modify(HorribleGJGameLevel, GJGameLevel)
{
    void savePercentage(int percent, bool isPracticeMode, int clicks, int attempts, bool isChkValid)
    {
        auto safeMode = Mod::get()->getSettingValue<bool>("safe-mode");

        if (!safeMode)
        {
            GJGameLevel::savePercentage(percent, isPracticeMode, clicks, attempts, isChkValid);
            return;
        }
        else
        {
            log::warn("Safe mode is enabled");
        };
    };
};