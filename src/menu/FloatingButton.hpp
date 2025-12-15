#pragma once

#include <Geode/Geode.hpp>
#include <Utils.hpp>

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

    static FloatingButton* create();

public:
    static FloatingButton* get();

    void setOpacity(GLubyte opacity);
    void setShowInLevel(bool show);

    void onEnter() override;

    void setScale(float scale) override;
    void setPosition(const CCPoint& position) override;

    // CCLayer targeted touch listener methods
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override;

    int64_t getOpacitySetting() const;
    float getScaleSetting() const;
    bool showInLevel() const;
};