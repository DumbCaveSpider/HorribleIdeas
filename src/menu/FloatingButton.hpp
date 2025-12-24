#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class FloatingButton : public CCLayer {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

protected:
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
    void setPosition(CCPoint const& position) override;

    // CCLayer targeted touch listener methods
    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override;

    int64_t getOpacitySetting() const;
    float getScaleSetting() const;
    bool showInLevel() const;
};