#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(MathQuizGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = options::get("math_quiz");
        int chance = options::getChance("math_quiz");

        Ref<MathQuiz> m_currentQuiz = nullptr;
        float m_nextQuizCooldown = 0.f;
    };

    void update(float p0) {
        int rnd = randng::fast();
        // log::debug("gjbasegamelayer update chance {}", rnd);

        if (m_fields->enabled) {
            // decrement cooldown timer before possibly showing a new quiz
            if (m_fields->m_nextQuizCooldown > 0.f) {
                m_fields->m_nextQuizCooldown -= p0;
                if (m_fields->m_nextQuizCooldown < 0.f) m_fields->m_nextQuizCooldown = 0.f;
            };

            if (rnd <= m_fields->chance) {
                // log::warn("richard was here");

                if (m_isPracticeMode && !m_fields->m_currentQuiz && m_fields->m_nextQuizCooldown <= 0.f) {
                    if (auto playLayer = PlayLayer::get()) {
#ifdef GEODE_IS_WINDOWS
                        CCEGLView::sharedOpenGLView()->showCursor(true);
#endif

                        if (auto quiz = MathQuiz::create()) {
#ifdef GEODE_IS_WINDOWS
                            CCEGLView::sharedOpenGLView()->showCursor(true);
#endif
                            m_fields->m_currentQuiz = quiz;
                            // clear pointer on close / handle correct/wrong answer
                            m_fields->m_currentQuiz->setOnCloseCallback([this]() {
                                if (!m_fields->m_currentQuiz) return;

                                if (!m_fields->m_currentQuiz->wasCorrect()) if (auto playLayer = PlayLayer::get()) playLayer->resetLevelFromStart();

                                // set cooldown between 5 and 10 seconds before next quiz
                                int rnd = randng::fast();
                                m_fields->m_nextQuizCooldown = static_cast<float>((rnd % 6) + 5); // 5..10 seconds

                                m_fields->m_currentQuiz = nullptr;
                                                                        });
                            playLayer->addChild(m_fields->m_currentQuiz, 1000);
                        };
                    };
                };
            };
        };

        GJBaseGameLayer::update(p0);
    };
};