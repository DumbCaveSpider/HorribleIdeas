#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(HorriblePlayerObject, PlayerObject) {
    void updateJump(float p0) {
        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("gravity", false)) {
            float newGrav = (static_cast<float>(std::rand()) / RAND_MAX); // modify gravity force

            // check if player touched any floor i think
            auto onGrnd = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
            auto minGrav = 0.125f; // minimum grav force

            if (newGrav < minGrav) newGrav = minGrav;

            onGrnd ? (m_gravityMod = 1.f * newGrav) : (m_gravityMod = 1.f);
            if (onGrnd) log::debug("set gravity to x{}", newGrav);
        };

        PlayerObject::updateJump(p0);
    };
};