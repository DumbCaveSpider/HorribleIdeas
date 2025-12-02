#include <Geode/Geode.hpp>
#include <Horrible.hpp>
#include <algorithm>
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
      std::uniform_int_distribution<> opDist(0, 2);

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

      // Create problem label
      std::string operationSymbol = (m_operation == 0) ? "+" : (m_operation == 1) ? "-"
                                                                                  : "*";
      std::string problemText = fmt::format("{} {} {}", m_numFirst, operationSymbol, m_numSecond);

      auto winSize = CCDirector::get()->getWinSize();
      auto problemLabel = CCLabelBMFont::create(problemText.c_str(), "goldFont.fnt");
      problemLabel->setID("problem-label");
      problemLabel->setPosition({winSize.width / 2.f, winSize.height - 60.f});
      problemLabel->setScale(1.5f);
      this->addChild(problemLabel);

      auto equalsLabel = CCLabelBMFont::create("= ?", "bigFont.fnt");
      equalsLabel->setID("equals-label");
      equalsLabel->setPosition({winSize.width / 2.f, winSize.height - 100.f});
      this->addChild(equalsLabel);

      // i hope i did this right cheese, u added this progress bar thing
      if (!m_timerBar) {
            m_timerBar = ProgressBar::create();
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

      // Add 3 wrong answers evil me
      std::uniform_int_distribution<> wrongDist(-20, 20);
      while (m_answers.size() < 4) {
            int wrongAnswer = m_correctAnswer + wrongDist(gen);
            // Make sure it's not the correct answer and not a duplicate
            if (wrongAnswer != m_correctAnswer &&
                std::find(m_answers.begin(), m_answers.end(), wrongAnswer) == m_answers.end()) {
                  m_answers.push_back(wrongAnswer);
            }
      }

      // Shuffle the answers
      std::shuffle(m_answers.begin(), m_answers.end(), gen);

      // Create answer buttons in a 2x2 grid
      auto menu = CCMenu::create();
      menu->setID("answer-menu");
      menu->setPosition({winSize.width / 2.f, winSize.height / 2.f - 20.f});

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

            // Position buttons in 2x2 grid
            float x = (i % 2 == 0) ? -spacingX / 2.f : spacingX / 2.f;
            float y = (i < 2) ? spacingY / 2.f : -spacingY / 2.f;

            answerBtn->setPosition({x, y});
            answerBtn->setTag(m_answers[i]);  // Store the answer value as tag
            menu->addChild(answerBtn);
      }

      this->addChild(menu);

      return true;
}

void MathQuiz::onAnswerClicked(CCObject* sender) {
      if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)) {
            int selectedAnswer = btn->getTag();
            bool correct = (selectedAnswer == m_correctAnswer);

            // Button shake for incorrect
            if (!correct) {
                  auto shake = CCSequence::create(
                      CCMoveBy::create(0.03f, ccp(-6, 0)),
                      CCMoveBy::create(0.06f, ccp(12, 0)),
                      CCMoveBy::create(0.03f, ccp(-6, 0)),
                      nullptr);
                  btn->runAction(shake);
            }

            // Create feedback label
            Notification::create(correct ? "Correct!" : "Incorrect!", correct ? NotificationIcon::Success : NotificationIcon::Error, 1.5f)->show();
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

            setWasCorrectFlag(correct);
            this->unscheduleUpdate();
      }
}

void MathQuiz::keyBackClicked() {
      setWasCorrectFlag(false);
      closePopup();
}

// onClose is not used by CCBlockLayer; prefer closePopup() helper instead

void MathQuiz::closeAfterFeedback(CCNode* node) {
      // clean up feedback label
      if (node) node->removeFromParentAndCleanup(true);
      // Call onClose callback if any
      if (m_onCloseCallback) m_onCloseCallback();
      this->unscheduleUpdate();
      removeFromParentAndCleanup(true);
}

void MathQuiz::closePopup() {
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
            Notification::create("Time's Up!", NotificationIcon::Error, 1.5f)->show();
            auto winSize = CCDirector::get()->getWinSize();
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
