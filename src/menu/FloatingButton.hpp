#pragma once

#include <Geode/Geode.hpp>
#include <Horrible.hpp>

using namespace geode::prelude;
using namespace horrible;

class FloatingButton : public CCLayer {
protected:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    FloatingButton();
    virtual ~FloatingButton();

    void onScaleEnd();

    virtual bool init() override;
    virtual void visit() override;

public:
    static FloatingButton* get();
    static FloatingButton* create();

    void setOpacity(GLubyte opacity);

    void onEnter() override;
    void onExit() override;

    void setScale(float scale) override;
    void setPosition(const CCPoint& position) override;

    // CCLayer targeted touch listener methods
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override;
};