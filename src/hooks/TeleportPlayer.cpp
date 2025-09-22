#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(TeleportPlayerObject, PlayerObject)
{
    struct Fields
    {
        bool enabled = horribleMod->getSavedValue<bool>("dementia", false);
        float chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("dementia-chance"));
        float lastX = 0.f; // last recorded X position
        float lastY = 0.f; // last recorded Y position
    };

    bool pushButton(PlayerButton p0)
    {
        if (auto playLayer = PlayLayer::get())
        {
            if (m_fields->enabled)
            {
                auto rnd = Rand::fast();
                log::debug("player teleport chance {}", rnd);

                auto onGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
                // dementia
                if (rnd <= m_fields->chance)
                {
                    setPosition(cocos2d::CCPoint(m_fields->lastX, m_fields->lastY));
                    log::debug("player has dementia to ({}, {})", m_fields->lastX, m_fields->lastY);
                    return PlayerObject::pushButton(p0);
                }
                else if (onGround) // save the position only if on ground
                {
                    m_fields->lastX = getPositionX();
                    m_fields->lastY = getPositionY();
                    log::debug("player position recorded to ({}, {})", m_fields->lastX, m_fields->lastY);
                }
            }
        }
        return PlayerObject::pushButton(p0);
    };
};