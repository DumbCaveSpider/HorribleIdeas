#include <ctime>
#include <cstdlib>

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

struct RandomSeeder
{
    RandomSeeder() { srand(time(0)); }
};
static RandomSeeder _randomSeeder;

class $modify(HorriblePlayerObject, PlayerObject)
{
    void updateJump(float p0)
    {
        auto horribleMod = getMod();
        auto rnd = rand() % 101;
        // log::debug("player object chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("gravity", false))
        {
            float newGrav = std::round((static_cast<float>(rnd) / 100.f) * (2.5f - 0.01f) * 100.0f) / 100.0f;

            // check if player touched any floor i think
            auto onGrnd = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;

            onGrnd ? (m_gravityMod = 1.f * newGrav) : (m_gravityMod = 1.f);
            if (onGrnd && !m_isRotating)
                log::debug("set gravity to x{}", newGrav);
        };

        PlayerObject::updateJump(p0);
    };
};