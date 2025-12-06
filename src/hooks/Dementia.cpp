#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/EnhancedGameObject.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(DementiaPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("dementia");
        int chance = options::getChance("dementia");

        int lastMusicTime = 0; // last music time in milliseconds

        float lastX = 0.f;     // last recorded X position
        float lastY = 0.f;     // last recorded Y position
    };

    bool pushButton(PlayerButton p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                int rnd = randng::fast();
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

class $modify(DementiaEnhancedGameObject, EnhancedGameObject) {
    struct Fields {
        bool enabled = options::get("dementia");
    };

    bool hasBeenActivated() {
        if (m_fields->enabled) {
            return true;
        } else {
            return EnhancedGameObject::hasBeenActivated();
        };
    };

    bool hasBeenActivatedByPlayer(PlayerObject * p0) {
        if (m_fields->enabled) {
            return true;
        } else {
            return EnhancedGameObject::hasBeenActivatedByPlayer(p0);
        };
    };
};