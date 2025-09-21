#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;



class $modify(HorribleGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        MathQuiz* m_currentQuiz = nullptr;
        RandomAdPopup* m_currentAd = nullptr;

        float m_adCooldown = 0.f;
    };

    void handleButton(bool down, int button, bool isPlayer1) {


        auto rnd = Rand::fast();
        log::debug("gjbasegamelayer handleButton chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("no_jump", false) && !m_isPlatformer) {
            if (button == 1) {
                if (rnd <= static_cast<int>(horribleMod->getSettingValue<int64_t>("math_quiz-chance"))) {
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
        auto rnd = Rand::fast();
        //log::debug("gjbasegamelayer update chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("math_quiz", false)) {
            if (rnd <= static_cast<int>(horribleMod->getSettingValue<int64_t>("math_quiz-chance"))) {
                log::warn("richard was here");

                if (m_isPracticeMode && !m_fields->m_currentQuiz) {
                    // pause the game
                    auto gm = GameManager::sharedState();
                    if (auto playLayer = PlayLayer::get()) playLayer->pauseGame(true);

                    if (auto quiz = MathQuiz::create()) {
#ifdef GEODE_IS_WINDOWS
                        CCEGLView::sharedOpenGLView()->showCursor(true);
#endif
                        m_fields->m_currentQuiz = quiz;
                        m_fields->m_currentQuiz->customSetup();

                        CCScene::get()->addChild(m_fields->m_currentQuiz);
                    };
                };
            };

            // do something like if answer is right, just do PlayLayer::resumeGame()
            // then do something like if answer is wrong, do PlayLayer::resetLevelFromStart()
            // if the user do a keyBack, then absolutely do PlayLayer::resetLevelFromStart() :trol:
        };

        if (horribleMod->getSavedValue<bool>("ads", false)) {
            m_fields->m_adCooldown += p0;

            static float nextAdTime = 0.f;
            auto gm = GameManager::sharedState();

            if (nextAdTime == 0.f || m_fields->m_adCooldown >= nextAdTime) {
                // Randomize next ad time between 20 and 40 seconds
                nextAdTime = 20.f + static_cast<float>(rand() % 21); // 20 to 40 inclusive
                m_fields->m_adCooldown = 0.f;

                log::warn("ad time!");

#ifdef GEODE_IS_WINDOWS
                // Show cursor when ad appears
                CCEGLView::sharedOpenGLView()->showCursor(true);
#endif
                if (!m_isPracticeMode && !m_fields->m_currentAd) {
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

#ifdef GEODE_IS_WINDOWS
            // Hide cursor if ad popup is closed
            if (!m_fields->m_currentAd && !CCScene::get()->getChildByID("ad"_spr)) CCEGLView::sharedOpenGLView()->showCursor(false);
#endif
        };

        GJBaseGameLayer::update(p0);
    };
};

class $modify(EvenWorseGJBaseGameLayer, GJBaseGameLayer) {
    bool init() {
        if (!GJBaseGameLayer::init()) return false;
        log::info("LOL init");
        return true;
    };
};