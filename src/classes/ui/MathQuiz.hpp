#pragma once

#include <Geode/Geode.hpp>
#include <functional>

using namespace geode::prelude;

namespace horrible {
    class MathQuiz : public CCBlockLayer, public FLAlertLayerProtocol {
    protected:
        int m_numFirst = 0;
        int m_numSecond = 0;
        int m_operation = 0; // 0 = add, 1 = subtract, 2 = multiply

        int m_correctAnswer = 0;
        std::vector<int> m_answers; // 4 answer options

        bool init() override;
        void onAnswerClicked(CCObject* sender);

        Ref<ProgressBar> m_timerBar = nullptr;
        Ref<CCMenu> m_answerMenu = nullptr;
        Ref<CCSprite> m_richardSprite = nullptr;
        float m_timeRemaining = 10.f;
        float m_totalTime = 10.f;
        std::function<void()> m_onCloseCallback = nullptr;
        bool m_wasCorrect = false;
        void keyBackClicked() override;
        void update(float dt) override;
        void onTimeout();
        void closeAfterFeedback(CCNode* node);

    public:
        void setOnCloseCallback(std::function<void()> cb) { m_onCloseCallback = cb; }
        void setWasCorrectFlag(bool v) { m_wasCorrect = v; }
        void closePopup();

    public:
        bool wasCorrect() const { return m_wasCorrect; }

    public:
        static MathQuiz* create();
    };
};