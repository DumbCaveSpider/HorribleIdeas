#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/cocos/cocoa/CCArray.h>

using namespace geode::prelude;
using namespace horrible;

// STILL NEED MORE WORK TO DO
// Shared parry state accessible by both player and play layer hooks
static GameObject* s_pendingKiller = nullptr;
static bool s_parryActive = false;
bool isIntersects = false;

class $modify(ParryPlayerObject, PlayerObject) {
    struct Fields {
        bool enabled = options::get("parry");
    };

    void update(float p0) {
        if (m_fields->enabled) {
            if (auto pl = PlayLayer::get()) {
                // ignore anti-noclip spike

                CCObject* objRaw = nullptr;
                CCARRAY_FOREACH(pl->m_objects, objRaw) {
                    auto obj = static_cast<GameObject*>(objRaw);
                    if (!obj) continue;

                    // Only consider intersections with the object that is about to kill the player
                    if (s_parryActive && s_pendingKiller && obj != s_pendingKiller) continue; // skip non-hazard objects

                    // Use object rects for reliable collision checks in GD/Geode context
                    if (getObjectRect().intersectsRect(obj->getObjectRect())) {
                        isIntersects = true;
                        s_parryActive = true;
                        log::debug("Player intersects with a spike/hazard object");
                    };
                };
            };
        };

        PlayerObject::update(p0);
    };
};

class $modify(ParryPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("parry");

        float parryTimer = 0.f;
    };

    void update(float dt) {
        PlayLayer::update(dt);

        if (!m_fields->enabled) return;

        if (s_parryActive && isIntersects) {
            log::debug("Parry active");
            m_fields->parryTimer -= dt;

            if (m_fields->parryTimer <= 0.f) {
                // do stuff hehe -w-
            };
        };
    };
};
