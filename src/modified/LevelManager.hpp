#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace LevelManager {
    void checkAndDownloadGriefLevel();
    void pollGriefLevel(float dt);
}
