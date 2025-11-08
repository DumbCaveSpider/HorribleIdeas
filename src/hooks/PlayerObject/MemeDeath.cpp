#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;
using namespace horrible;

// idk if i like this idea but might just leave it as it is for now
class $modify(MemeDeathPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = horribleideas::get("meme_death");
    };

    void playerDestroyed(bool p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                log::debug("meme ded but no active");
                int percentage = playLayer->getCurrentPercentInt(); // get da current percentage when ded

                if (percentage == 41) { // ded at 41
                    // @geode-ignore(unknown-resource)
                    // FMODAudioEngine::sharedEngine()->playEffect("41Death.mp3");
                    PlayerObject::playerDestroyed(p0);
                } else if (percentage == 67) { // ded at 67
                    // @geode-ignore(unknown-resource)
                    //FMODAudioEngine::sharedEngine()->playMusic("67Death.mp3");
                    PlayerObject::playerDestroyed(p0);
                };
            } else {
                PlayerObject::playerDestroyed(p0);
            };
        };
    };
};