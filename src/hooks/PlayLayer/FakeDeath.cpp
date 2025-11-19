#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FakeDeathPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("death");

        bool m_dontCreateObjects = false;
        GameObject* m_destroyingObject;
    };

    void destroyPlayer(PlayerObject * player, GameObject * game) {
        // Show explosion visual effect but do not kill the player
        if (m_fields->enabled) {
            // ignore the anti-cheat spike lmao
            if (game == m_anticheatSpike && player && !player->m_isDead) return;
            if (!m_fields->m_destroyingObject) m_fields->m_destroyingObject = game;

            // @geode-ignore(unknown-resource)
            FMODAudioEngine::sharedEngine()->playEffect("explode_11.ogg");
            GJBaseGameLayer::shakeCamera(1.f, 2.f, 1.f);

            if (m_player1) {
                log::debug("fake death");
                m_player1->playDeathEffect();
                m_player1->resetPlayerIcon();

                m_player1->m_isDead = false;
                return;
            };

            if (m_player2) {
                log::debug("fake death");
                m_player2->playDeathEffect();
                m_player2->resetPlayerIcon();

                m_player2->m_isDead = false;
                return;
            };
        };

        PlayLayer::destroyPlayer(player, game);
    };
};