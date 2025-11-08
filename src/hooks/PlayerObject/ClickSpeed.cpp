#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/GameManager.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(ClickSpeedPlayerObject, PlayerObject)
{
    struct Fields
    {
        bool enabled = horribleideas::get("click_speed");
    };

    bool pushButton(PlayerButton p0)
    {
        if (m_fields->enabled)
        {
            int rng = randng::fast() % 100;
            int currentSpeed = this->m_playerSpeed;
            log::debug("current speed: {}", currentSpeed);
            if (rng <= 50)
            {
                // increase the player speed
                this->m_playerSpeed = currentSpeed + 1;
            }
            else
            {
                // decrease the player speed
                this->m_playerSpeed = currentSpeed - 1;
            }
            log::debug("Click Speed modified player speed to {}", this->m_playerSpeed);
        }
        return PlayerObject::pushButton(p0);
    }
};