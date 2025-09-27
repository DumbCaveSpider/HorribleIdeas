#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(DementiaPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = horribleMod->getSavedValue<bool>("dementia", false);
        float chance = static_cast<int>(horribleMod->getSettingValue<int64_t>("dementia-chance"));

        int lastMusicTime = 0; // last music time in milliseconds

        float lastX = 0.f;     // last recorded X position
        float lastY = 0.f;     // last recorded Y position
    };

    bool pushButton(PlayerButton p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                auto rnd = Rand::fast();
                log::debug("player teleport chance {}", rnd);

                FMOD::Channel* musicChannel = nullptr;
                auto fmod = FMODAudioEngine::sharedEngine();

                auto bgchannel = fmod->m_backgroundMusicChannel;
                auto channel = bgchannel->getChannel(0, &musicChannel);

                auto onGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;
                // dementia
                if (rnd <= m_fields->chance) {
                    setPosition(cocos2d::CCPoint(m_fields->lastX, m_fields->lastY));
                    log::debug("player has dementia to ({}, {}), play time {}", m_fields->lastX, m_fields->lastY, m_fields->lastMusicTime);

                    // set the music time back to the last recorded time
                    if (musicChannel) musicChannel->setPosition(m_fields->lastMusicTime, FMOD_TIMEUNIT_MS);

                    return PlayerObject::pushButton(p0);
                } else if (onGround) { // save the position only if on ground
                    m_fields->lastX = getPositionX();
                    m_fields->lastY = getPositionY();
                    m_fields->lastMusicTime = fmod->getMusicTimeMS(1);

                    log::debug("position recorded to ({}, {}) and music time {}", m_fields->lastX, m_fields->lastY, m_fields->lastMusicTime);
                };
            };
        };

        return PlayerObject::pushButton(p0);
    };
};