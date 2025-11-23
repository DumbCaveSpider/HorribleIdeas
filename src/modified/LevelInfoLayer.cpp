#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorribleLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel * level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (auto leftMenu = getChildByID("left-side-menu")) menu::addButton(leftMenu);

        return true;
    };
};