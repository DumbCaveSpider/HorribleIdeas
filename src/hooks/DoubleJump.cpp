#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(DoubleJumpPlayerObject, PlayerObject) {
    struct Fields {
        int m_jumps = 0;
    };

    bool pushButton(PlayerButton p0) {
        if (horribleMod->getSavedValue<bool>("double_jump", false)) {
            if (p0 == PlayerButton::Jump) {
                if (m_isOnGround) m_fields->m_jumps = 0;
                if (!m_isOnGround) m_fields->m_jumps++;
            };

            m_isOnGround = m_fields->m_jumps < 2;
        };

        return PlayerObject::pushButton(p0);
    };
};