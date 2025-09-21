#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

static RandomSeeder _randomSeeder;

class $modify(GravityPlayerObject, PlayerObject) {
    struct Fields {
        bool m_isOnSlope = false;
        bool m_wasOnSlope = false;
    };

    void updateJump(float p0) {
        auto rnd = rand() % 101;

        if (horribleMod->getSavedValue<bool>("gravity", false)) {
            float newGrav = std::round((static_cast<float>(rnd) / 100.f) * (2.5f) * 100.0f) / 100.0f;
            auto onGrnd = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;

            // Only set gravity if on flat ground (not on a slope) and not rotating
            if (onGrnd && !m_isRotating && !m_fields->m_isOnSlope && !m_fields->m_wasOnSlope) {
                m_gravityMod = newGrav;
                log::debug("set gravity to x{} (flat ground)", newGrav);
            };
        };

        PlayerObject::updateJump(p0);
    };
};