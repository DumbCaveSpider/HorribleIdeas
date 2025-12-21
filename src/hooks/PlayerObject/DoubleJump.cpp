#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(DoubleJumpPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("double_jump");

        int m_jumps = 0;
    };

    bool pushButton(PlayerButton p0) {
        if (m_fields->enabled) {
            if (p0 == PlayerButton::Jump) {
                if (m_isOnGround) m_fields->m_jumps = 0;
                if (!m_isOnGround) m_fields->m_jumps++;
            };

            m_isOnGround = m_fields->m_jumps < 2;
        };

        return PlayerObject::pushButton(p0);
    };
};