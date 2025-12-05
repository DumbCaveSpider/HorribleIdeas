#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorribleCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        // if (auto bottomLeftMenu = getChildByID("bottom-left-menu")) menu::addButton(bottomLeftMenu, 0.875f);

        return true;
    };
};