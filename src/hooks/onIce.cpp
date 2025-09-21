#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(OnIcePlayerObject, PlayerObject) {
    void update(float p0) {
        // make the player always on ice. yea this is it xD
        if (horribleMod->getSavedValue<bool>("ice_level", false)) m_isOnIce = true;

        PlayerObject::update(p0);
    };
};