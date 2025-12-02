#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Horrible.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(MathQuizGJBaseGameLayer, GJBaseGameLayer) {
      struct Fields {
            bool enabled = options::get("math_quiz");
            int chance = options::getChance("math_quiz");

            MathQuiz* m_currentQuiz = nullptr;
      };

      void update(float p0) {
            auto rnd = randng::fast();
            // log::debug("gjbasegamelayer update chance {}", rnd);

            if (m_fields->enabled) {
                  if (rnd <= m_fields->chance) {
                        //log::warn("richard was here");

                        if (m_isPracticeMode && !m_fields->m_currentQuiz) {
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
                                                bool correct = m_fields->m_currentQuiz->wasCorrect();
                                                if (!correct) {
                                                      if (auto playLayer = PlayLayer::get()) playLayer->resetLevelFromStart();
                                                }

                                                m_fields->m_currentQuiz = nullptr;
                                          });
                                          playLayer->addChild(m_fields->m_currentQuiz, 1000);
                                    }
                              }
                        }
                  }
            }

            GJBaseGameLayer::update(p0);
      }
};