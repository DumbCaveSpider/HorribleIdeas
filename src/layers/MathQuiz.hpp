#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MathQuiz : public CCBlockLayer, public FLAlertLayerProtocol {
protected:
    int m_numFirst = 1; // First number in the equation
    int m_numSecond = 1; // Second number in the equation

    int m_answer = 1; // The calculated answer to the equation

    TextInput* m_answerInput = nullptr; // Answer gets typed out here

    void infoPopup(CCObject*);

    bool init() override;

public:
    static MathQuiz* create();
    void keyBackClicked() override;
};