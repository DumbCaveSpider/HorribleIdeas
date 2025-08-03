#include "../RandomSeeder.hpp"

#include "../layers/MathQuiz.hpp"
#include "../popups/RandomAdPopup.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

static RandomSeeder _randomSeeder;

class $modify(HorribleGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        MathQuiz* m_currentQuiz = nullptr;
        RandomAdPopup* m_currentAd = nullptr;

        float m_adCooldown = 0.f;
    };

    void handleButton(bool down, int button, bool isPlayer1) {
        auto horribleMod = getMod();

        if (horribleMod->getSavedValue<bool>("no-jump", false) && !m_isPlatformer) {
            if (button == 1) {
                if ((rand() % 101) <= 11) {
                    log::error("skipped dat jump");
                    GJBaseGameLayer::handleButton(false, button, isPlayer1);
                } else {
                    log::debug("registered dat jump");
                };
            };
        } else {
            GJBaseGameLayer::handleButton(down, button, isPlayer1);
        };
    };

    void update(float p0) {
        auto horribleMod = getMod();

        auto rnd = rand() % 101;
        log::debug("gjbasegamelayer update chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("math-quiz", false)) {
            if (rnd <= 1) {
                log::warn("richard was here");

                if (m_isPracticeMode && !m_fields->m_currentQuiz) {
                    if (auto quiz = MathQuiz::create()) {
                        m_fields->m_currentQuiz = quiz;
                        m_fields->m_currentQuiz->customSetup();

                        CCScene::get()->addChild(m_fields->m_currentQuiz);
                    };
                };
            };
        };

        if (horribleMod->getSavedValue<bool>("ads", false)) {
            m_fields->m_adCooldown += p0; // continue til it hits 10 sec cooldown
            if (m_fields->m_adCooldown >= 10.f) m_fields->m_adCooldown = 0.f;

            if (rnd <= 1) {
                log::warn("ad time!");

                if (!m_isPracticeMode && !m_fields->m_currentAd && m_fields->m_adCooldown < 10.f) {
                    if (auto popup = RandomAdPopup::create()) {
                        m_fields->m_currentAd = popup;
                        m_fields->m_currentAd->show();
                    };
                };

                // if there's already an ad muahahahaha
                if (auto alreadyPopup = CCScene::get()->getChildByID("ad"_spr)) {
                    log::debug("there's already an ad!");
                } else {
                    m_fields->m_currentAd = nullptr;
                };
            };
        };

        GJBaseGameLayer::update(p0);
    };
};