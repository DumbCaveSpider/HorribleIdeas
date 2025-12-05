#pragma once

#include <Geode/Geode.hpp>
#include <Horrible.hpp>

using namespace geode::prelude;
using namespace horrible;

class FloatingButton : public CCLayer {
     protected:
      bool isDragging = false;
      bool isMoving = false;
      CCPoint dragStartPos = {0, 0};
      CircleButtonSprite* m_sprite = nullptr;

      public:
       static FloatingButton* get();
       static FloatingButton* create();

      void setPosition(const CCPoint& position) override;

      // CCLayer targeted touch listener methods
      virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
      virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
      virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override;

      virtual void update(float delta) override;
      virtual bool init() override;
      virtual void visit() override;
};