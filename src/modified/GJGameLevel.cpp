#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorribleGJGameLevel, GJGameLevel) {
    void savePercentage(int percent, bool isPracticeMode, int clicks, int attempts, bool isChkValid) {
        if (horribleMod->getSettingValue<bool>("safe-mode")) {
            log::warn("Safe mode is enabled, progress will not be saved");
        } else {
            GJGameLevel::savePercentage(percent, isPracticeMode, clicks, attempts, isChkValid);
        };
    };
};