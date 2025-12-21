#pragma once

#include <Geode/Geode.hpp>
#include <functional>

using namespace geode::prelude;

namespace horrible {
    enum class MathOperation {
        Addition = 0,
        Subtraction = 1,
        Multiplication = 2,
        Geometry = 3
    };

    class MathQuiz : public CCBlockLayer, public FLAlertLayerProtocol {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        MathQuiz();
        virtual ~MathQuiz();

        void onTimeout();

        void closeAfterFeedback(CCNode* node);
        void onAnswerClicked(CCObject* sender);

        void keyBackClicked() override;
        void update(float dt) override;

        bool init() override;

    public:
        static MathQuiz* create();

        void setOnCloseCallback(std::function<void()> cb);
        void setWasCorrectFlag(bool v);

        void closePopup();

        bool wasCorrect() const;
    };

    class Richard : public CCNode {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        Richard();
        virtual ~Richard();

        bool init() override;

    public:
        static Richard* create();
    };
};