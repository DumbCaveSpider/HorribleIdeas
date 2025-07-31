#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MathQuiz : public CCLayer {
protected:
    TextInput* m_answerInput = nullptr; // Answer gets typed out here

    bool init();

    void keyBackClicked() override;
    void onGoBack(CCObject*);

    void infoPopup(CCObject*);

public:
    static MathQuiz* create();
    static MathQuiz* scene();
};