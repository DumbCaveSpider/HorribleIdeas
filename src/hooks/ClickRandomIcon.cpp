#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/GameManager.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(ClickRandomIconPlayerObject, PlayerObject) {
    bool pushButton(PlayerButton p0) {
        if (horribleMod->getSavedValue<bool>("random_icon", false)) {
            // jump pls
            if (auto playLayer = PlayLayer::get(); playLayer && p0 == PlayerButton::Jump) {
                auto gm = GameManager::sharedState();

                // count the icons i guess
                int maxIcons = 0;
                if (gm) maxIcons = gm->countForType(IconType::Cube);
                if (maxIcons <= 0) maxIcons = 38;

                // pick random icons that is unlocked
                int tries = 0;
                int newIcon = rand() % maxIcons + 1;
                while (tries < 20 && gm && !gm->isIconUnlocked(newIcon, IconType::Cube)) {
                    newIcon = rand() % maxIcons + 1;
                    ++tries;
                }

                // randomize the colors of the icon
                int r = rand() % 256;
                int g = rand() % 256;
                int b = rand() % 256;
                setColor(cocos2d::ccc3(r, g, b));
                updatePlayerGlow();


                updatePlayerFrame(newIcon);
                log::debug("Changed player icon to {}", newIcon);
            }
        }

        return PlayerObject::pushButton(p0);
    };
};