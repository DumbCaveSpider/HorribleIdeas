#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(MathQuizGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = horribleideas::get("math_quiz");
        int chance = horribleideas::getChance("math_quiz");

        MathQuiz* m_currentQuiz = nullptr;
    };

    void update(float p0) {
        auto rnd = Rand::fast();
        // log::debug("gjbasegamelayer update chance {}", rnd);

        if (m_fields->enabled) {
            if (rnd <= m_fields->chance) {
                log::warn("richard was here");

                if (m_isPracticeMode && !m_fields->m_currentQuiz) {
                    // pause the game
                    auto gm = GameManager::sharedState();
                    if (auto playLayer = PlayLayer::get())
                        playLayer->pauseGame(true);

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

        GJBaseGameLayer::update(p0);
    };
};