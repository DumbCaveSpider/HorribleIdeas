#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorribleEditLevelLayer, EditLevelLayer) {
    bool init(GJGameLevel * level) {
        if (!EditLevelLayer::init(level)) return false;

        if (auto levelActionsMenu = getChildByID("level-actions-menu")) menu::addButton(levelActionsMenu);

        return true;
    };
};