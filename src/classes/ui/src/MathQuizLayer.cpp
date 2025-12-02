#include <Geode/Geode.hpp>
#include <Horrible.hpp>
#include <algorithm>
#include <cmath>
#include <random>

#include "../MathQuiz.hpp"

using namespace geode::prelude;
using namespace horrible;

bool MathQuiz::init() {
      if (!CCBlockLayer::init()) return false;
      setID("math_quiz"_spr);

      // gen random numbers and operation
      // holy ass c++ rnd i hate this
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> numDist(1, 10);
      std::uniform_int_distribution<> opDist(0, 3);

      m_numFirst = numDist(gen);
      m_numSecond = numDist(gen);
      m_operation = opDist(gen);

      switch (m_operation) {
            case 0:  // Addition
                  m_correctAnswer = m_numFirst + m_numSecond;
                  break;
            case 1:  // Subtraction
                  m_correctAnswer = m_numFirst - m_numSecond;
                  break;
            case 2:  // Multiplication
                  m_correctAnswer = m_numFirst * m_numSecond;
                  break;
      }

      auto winSize = CCDirector::get()->getWinSize();
      // geometry dash
      if (m_operation == 3) {
            std::uniform_int_distribution<> sidesDist(3, 8);
            int sides = sidesDist(gen);
            m_correctAnswer = sides;

            // draw node and polygon points
            if (!m_drawNode) {
                  m_drawNode = CCDrawNode::create();
                  m_drawNode->setID("math-quiz-drawnode"_spr);
            }

            float radius = std::min(winSize.width, winSize.height) / 8.f;
            float centerX = winSize.width / 2.f + 160.f; // offset a bit to the right
            float centerY = winSize.height / 2.f;

            std::vector<CCPoint> polyPoints;
            polyPoints.reserve(sides);
            constexpr float PI = 3.14159265358979323846f;
            float theta = (2.0f * PI) / sides;
            for (int i = 0; i < sides; ++i) {
                  float angle = theta * i - PI / 2.0f; // start at top
                  float x = radius * cosf(angle);
                  float y = radius * sinf(angle);
                  polyPoints.push_back(ccp(x, y));
            }

            // draw the polygon in local coords with drawNode placed at center
            m_drawNode->setPosition({centerX, centerY});
            cocos2d::ccColor4F fillColor = {0.85f, 0.65f, 0.15f, 1.0f};
            cocos2d::ccColor4F borderColor = {0.05f, 0.05f, 0.05f, 1.0f};
            m_drawNode->clear();
            m_drawNode->drawPolygon(polyPoints.data(), static_cast<unsigned int>(polyPoints.size()), fillColor, 2.0f, borderColor);
            this->addChild(m_drawNode, 250);

            // Set problem text for geometry
            // We'll override the problem label text later so set a generic label for now
      }

      // Create problem label
      std::string problemText;
      if (m_operation == 3) {
            problemText = "How many sides does this shape have?";
      } else {
            std::string operationSymbol = (m_operation == 0) ? "+" : (m_operation == 1) ? "-" : "*";
            problemText = fmt::format("{} {} {}", m_numFirst, operationSymbol, m_numSecond);
      }

      // reuse winSize declared above
      auto problemLabel = CCLabelBMFont::create(problemText.c_str(), "goldFont.fnt");
      problemLabel->setID("problem-label");
      problemLabel->setPosition({winSize.width / 2.f, winSize.height - 60.f});
      problemLabel->setScale(1.5f);
      this->addChild(problemLabel);

      if (m_operation != 3) {
            auto equalsLabel = CCLabelBMFont::create("= ?", "bigFont.fnt");
            equalsLabel->setID("equals-label");
            equalsLabel->setPosition({winSize.width / 2.f, winSize.height - 100.f});
            this->addChild(equalsLabel);
      }

      // i hope i did this right cheese, u added this progress bar thing
      if (!m_timerBar) {
            m_timerBar = ProgressBar::create();
            m_timerBar->setStyle(ProgressBarStyle::Solid);
            m_timerBar->setID("math-quiz-timer"_spr);
            m_timerBar->setFillColor({255, 210, 0});
            m_timerBar->setAnchorPoint({0.5f, 0.5f});
            m_timerBar->setPosition({winSize.width / 2.f, winSize.height - 20.f});
            m_timerBar->setZOrder(200);
            addChild(m_timerBar);
      }
      m_timeRemaining = m_totalTime = 10.f;
      m_timerBar->updateProgress(100.f);
      this->scheduleUpdate();

      // Generate 4 answer options with the correct answer randomized
      m_answers.clear();
      m_answers.push_back(m_correctAnswer);

      // Add 3 wrong answers
      if (m_operation == 3) {
            std::uniform_int_distribution<> wrongSidesDist(3, 8);
            while (m_answers.size() < 4) {
                  int wrongAnswer = wrongSidesDist(gen);
                  if (wrongAnswer != m_correctAnswer &&
                      std::find(m_answers.begin(), m_answers.end(), wrongAnswer) == m_answers.end()) {
                        m_answers.push_back(wrongAnswer);
                  }
            }
      } else {
            std::uniform_int_distribution<> wrongDist(-20, 20);
            while (m_answers.size() < 4) {
                  int wrongAnswer = m_correctAnswer + wrongDist(gen);
                  if (wrongAnswer != m_correctAnswer &&
                      std::find(m_answers.begin(), m_answers.end(), wrongAnswer) == m_answers.end()) {
                        m_answers.push_back(wrongAnswer);
                  }
            }
      }

      // Shuffle the answers
      std::shuffle(m_answers.begin(), m_answers.end(), gen);

      // richard floating lol
      if (!m_richardSprite) {
            auto richard = CCSprite::create("richard.png"_spr);
            if (richard) {
                  m_richardSprite = richard;
                  m_richardSprite->setID("richard_sprite"_spr);
                  m_richardSprite->setAnchorPoint({1.f, 0.5f});
                  m_richardSprite->setScale(0.6f);
                  m_richardSprite->setPosition({winSize.width - 36.f, winSize.height / 2.f});
                  m_richardSprite->setZOrder(500);
                  this->addChild(m_richardSprite);

                  // floating animation
                  float floatDistance = 8.f;
                  float floatTime = 1.0f;
                  auto moveUp = CCMoveBy::create(floatTime, ccp(0, floatDistance));
                  auto moveDown = CCMoveBy::create(floatTime, ccp(0, -floatDistance));
                  auto seq = CCSequence::create(moveUp, moveDown, nullptr);
                  auto repeat = CCRepeatForever::create(seq);
                  m_richardSprite->runAction(repeat);
            }
      }

      m_answerMenu = CCMenu::create();
      m_answerMenu->setID("answer-menu");
      m_answerMenu->setPosition({winSize.width / 2.f, winSize.height / 2.f - 20.f});

      float buttonWidth = 80.f;
      float buttonHeight = 35.f;
      float spacingX = 100.f;
      float spacingY = 50.f;

      for (int i = 0; i < 4; i++) {
            auto btnSprite = ButtonSprite::create(
                fmt::format("{}", m_answers[i]).c_str(),
                buttonWidth,
                true,
                "bigFont.fnt",
                "GJ_button_01.png",
                0,
                0.8f);

            auto answerBtn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(MathQuiz::onAnswerClicked));

            //  2x2 grid
            float x = (i % 2 == 0) ? -spacingX / 2.f : spacingX / 2.f;
            float y = (i < 2) ? spacingY / 2.f : -spacingY / 2.f;

            answerBtn->setPosition({x, y});
            answerBtn->setTag(m_answers[i]);
            m_answerMenu->addChild(answerBtn);
      }

      this->addChild(m_answerMenu);

      return true;
}

void MathQuiz::onAnswerClicked(CCObject* sender) {
      if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)) {
            int selectedAnswer = btn->getTag();
            bool correct = (selectedAnswer == m_correctAnswer);

            // Button shake for incorrect fancy
            if (!correct) {
                  auto shake = CCSequence::create(
                      CCMoveBy::create(0.03f, ccp(-6, 0)),
                      CCMoveBy::create(0.06f, ccp(12, 0)),
                      CCMoveBy::create(0.03f, ccp(-6, 0)),
                      nullptr);
                  btn->runAction(shake);
            }

            if (m_answerMenu) {
                  m_answerMenu->removeFromParentAndCleanup(true);
                  m_answerMenu = nullptr;
            }

            // feedback label
            // Notification::create(correct ? "Correct!" : "Incorrect!", correct ? NotificationIcon::Success : NotificationIcon::Error, 1.5f)->show();
            auto winSize = CCDirector::get()->getWinSize();
            auto feedbackLabel = CCLabelBMFont::create(correct ? "Correct!" : "Incorrect!", "goldFont.fnt");
            feedbackLabel->setID("feedback-label");
            feedbackLabel->setAnchorPoint({0.5f, 0.5f});
            feedbackLabel->setPosition({winSize.width / 2.f, winSize.height / 2.f});
            feedbackLabel->setScale(0.1f);

            if (!correct) {
                  feedbackLabel->setColor({255, 100, 100});
            } else {
                  feedbackLabel->setColor({100, 255, 100});
            }

            this->addChild(feedbackLabel, 1000);

            // Small scale animation, delay, then call close
            auto seq = CCSequence::create(
                CCScaleTo::create(0.12f, 1.2f),
                CCScaleTo::create(0.08f, 1.0f),
                CCDelayTime::create(0.75f),
                CCCallFuncN::create(this, callfuncN_selector(MathQuiz::closeAfterFeedback)),
                nullptr);

            feedbackLabel->runAction(seq);
            setKeypadEnabled(false);
            setWasCorrectFlag(correct);
            this->unscheduleUpdate();
      }
}

void MathQuiz::keyBackClicked() {
      if (m_answerMenu) {
            m_answerMenu->removeFromParentAndCleanup(true);
            m_answerMenu = nullptr;
      }
      if (m_drawNode) {
            m_drawNode->removeFromParentAndCleanup(true);
            m_drawNode = nullptr;
      }
      Notification::create("Nope! you cant escape the math quiz!", NotificationIcon::Error, 1.5f)->show();
      setWasCorrectFlag(false);
      closePopup();
}

void MathQuiz::closeAfterFeedback(CCNode* node) {
      if (node) node->removeFromParentAndCleanup(true);
      if (m_answerMenu) {
            m_answerMenu->removeFromParentAndCleanup(true);
            m_answerMenu = nullptr;
      }
      if (m_drawNode) {
            m_drawNode->removeFromParentAndCleanup(true);
            m_drawNode = nullptr;
      }
      if (m_onCloseCallback) m_onCloseCallback();
      this->unscheduleUpdate();
      removeFromParentAndCleanup(true);
}

void MathQuiz::closePopup() {
      if (m_answerMenu) {
            m_answerMenu->removeFromParentAndCleanup(true);
            m_answerMenu = nullptr;
      }
      if (m_onCloseCallback) m_onCloseCallback();
      this->unscheduleUpdate();
      removeFromParentAndCleanup(true);
      
}

void MathQuiz::update(float dt) {
      if (m_timeRemaining <= 0.f) return;
      m_timeRemaining -= dt;
      if (m_timeRemaining < 0.f) m_timeRemaining = 0.f;
      float pct = (m_timeRemaining / m_totalTime) * 100.f;
      if (m_timerBar) m_timerBar->updateProgress(pct);

      if (m_timeRemaining <= 0.f) {
            // automatic incorrect
            this->unscheduleUpdate();
            setWasCorrectFlag(false);
            // Notification::create("Time's Up!", NotificationIcon::Error, 1.5f)->show();
            auto winSize = CCDirector::get()->getWinSize();
            if (m_answerMenu) {
                  m_answerMenu->removeFromParentAndCleanup(true);
                  m_answerMenu = nullptr;
            }

            auto feedbackLabel = CCLabelBMFont::create("Time's Up!", "goldFont.fnt");
            feedbackLabel->setID("feedback-label");
            feedbackLabel->setAnchorPoint({0.5f, 0.5f});
            feedbackLabel->setPosition({winSize.width / 2.f, winSize.height / 2.f});
            feedbackLabel->setScale(0.1f);
            feedbackLabel->setColor({255, 100, 100});
            this->addChild(feedbackLabel, 1000);

            auto seq = CCSequence::create(
                CCScaleTo::create(0.12f, 1.2f),
                CCScaleTo::create(0.08f, 1.0f),
                CCDelayTime::create(0.75f),
                CCCallFuncN::create(this, callfuncN_selector(MathQuiz::closeAfterFeedback)),
                nullptr);

            feedbackLabel->runAction(seq);
      }
}

MathQuiz* MathQuiz::create() {
      auto ret = new MathQuiz();
      if (ret->init()) {
            ret->autorelease();
            return ret;
      }

      CC_SAFE_DELETE(ret);
      return nullptr;
}
