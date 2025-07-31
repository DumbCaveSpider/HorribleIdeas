#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(HorriblePlayerObject, PlayerObject) {
    void updateJump(float p0) {
        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("gravity", false)) {
            // Random gravity between 0.125f and 5.f
            float newGrav = 0.125f + (static_cast<float>(std::rand()) / RAND_MAX) * (2.5f - 0.125f);

            // check if player touched any floor i think
            auto onGrnd = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;

            onGrnd ? (m_gravityMod = 1.f * newGrav) : (m_gravityMod = 1.f);
            if (onGrnd) log::debug("set gravity to x{}", newGrav);
        };

        PlayerObject::updateJump(p0);
    };
};