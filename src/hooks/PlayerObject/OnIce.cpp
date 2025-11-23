#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(OnIcePlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("ice_level");
    };

    void update(float p0) {
        // make the player always on ice. yea this is it xD
        m_isOnIce = m_fields->enabled;

        PlayerObject::update(p0);
    };
};