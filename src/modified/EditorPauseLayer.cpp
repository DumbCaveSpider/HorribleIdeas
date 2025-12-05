#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorribleEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer * layer) {
        if (!EditorPauseLayer::init(layer)) return false;

        // if (auto guidelinesMenu = getChildByID("guidelines-menu")) menu::addButton(guidelinesMenu, 0.875f);

        return true;
    };
};