#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorribleMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (auto gm = GameManager::get()) {
            // get and store user current fps
            float currentFPS = gm->m_customFPSTarget;
            float storedFPS = horribleMod->setSavedValue<float>("fps", currentFPS);

            log::debug("Store Current FPS: {}", storedFPS);
        };

        if (auto bottomMenu = getChildByID("bottom-menu")) menu::addButton(bottomMenu, 1.125f);

        return true;
    };
};