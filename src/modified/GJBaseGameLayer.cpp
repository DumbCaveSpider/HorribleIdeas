#include "../RandomSeeder.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

static RandomSeeder _randomSeeder;

class $modify(HorribleGJBaseGameLayer, GJBaseGameLayer) {
    void handleButton(bool down, int button, bool isPlayer1) {
        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("no-jump", false) && !m_isPlatformer) {
            if (button == 1) {
                if ((rand() % 101) <= 11) {
                    log::error("skipped dat jump");
                    GJBaseGameLayer::handleButton(false, button, isPlayer1);
                } else {
                    log::debug("registered dat jump");
                    GJBaseGameLayer::handleButton(down, button, isPlayer1);
                };
            };
        } else {
            GJBaseGameLayer::handleButton(down, button, isPlayer1);
        };
    };
};