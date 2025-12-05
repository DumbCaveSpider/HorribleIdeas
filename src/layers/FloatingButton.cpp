#include "FloatingButton.hpp"

using namespace geode::prelude;
using namespace horrible;

FloatingButton* FloatingButton::create() {
      auto ret = new FloatingButton();
      if (ret && ret->init()) {
            ret->autorelease();
            return ret;
      }

      CC_SAFE_DELETE(ret);
      return nullptr;
}

FloatingButton* FloatingButton::get() {
      static FloatingButton* instance = nullptr;
      if (!instance) {
            instance = new FloatingButton();
            if (!instance->init()) {
                  delete instance;
                  instance = nullptr;
                  return nullptr;
            }
            instance->retain();
      }
      return instance;
}

bool FloatingButton::init() {
      // init as a layer so we can handle touches
      if (!CCLayer::init())
            return false;

      // get the saved position
      float x = horribleMod->getSavedValue<float>("button-x", 100.0f);
      float y = horribleMod->getSavedValue<float>("button-y", 100.0f);

      // create a circle sprite for the visual button
      auto sprite = CircleButtonSprite::createWithSprite(
          "shocksprite.png"_spr,
          0.9f);
      sprite->setScale(1.0f);
      sprite->setPosition({sprite->getContentSize().width / 2, sprite->getContentSize().height / 2});
      sprite->setOpacity(Mod::get()->getSettingValue<int64_t>("opacity-floating-button"));
      this->setContentSize(sprite->getContentSize() + 5);
      this->addChild(sprite);

      // set position
      this->setPosition({x, y});

      // enable touches and schedule updates
      this->setTouchMode(kCCTouchesOneByOne);
      this->setTouchEnabled(true);
      this->setTouchPriority(-1000);  // ewww touch priority
      this->setID("floating-button"_spr);
      this->setZOrder(999);
      this->scheduleUpdate();

      return true;
}

void FloatingButton::setPosition(const CCPoint& position) {
      CCLayer::setPosition(position);
      // save the position
      horribleMod->setSavedValue<float>("button-x", position.x);
      horribleMod->setSavedValue<float>("button-y", position.y);
}

// removed getPosition override; using base CCNode/CCLayer method instead

bool FloatingButton::ccTouchBegan(CCTouch* touch, CCEvent* ev) {
      CCPoint touchLocation = touch->getLocation();
      CCPoint nodeLocation = this->getPosition();
      float radius = 30.0f;  // small grab radius for the button

      if (ccpDistance(touchLocation, nodeLocation) <= radius) {
            isDragging = true;
            dragStartPos = ccpSub(nodeLocation, touchLocation);
            return true;  // swallow touch
      }
      return false;
}

void FloatingButton::ccTouchMoved(CCTouch* touch, CCEvent* ev) {
      if (isDragging) {
            CCPoint touchLocation = touch->getLocation();
            CCPoint newLocation = ccpAdd(touchLocation, dragStartPos);
            this->setPosition(newLocation);
            isMoving = true;
            // CCscaleTo action for visual feedback
            this->runAction(CCScaleTo::create(0.2f, 1.1f));
      }
}
void FloatingButton::ccTouchEnded(CCTouch* touch, CCEvent* ev) {
      // if we didn't move, treat as click
      if (!isMoving) {
            menu::open();
      }
      // reset state
      isDragging = false;
      isMoving = false;
      // store position
      horribleMod->setSavedValue<float>("button-x", this->getPosition().x);
      horribleMod->setSavedValue<float>("button-y", this->getPosition().y);
      // reset scale
      this->runAction(CCScaleTo::create(0.2f, 1.0f));
}

void FloatingButton::update(float delta) {
      CCLayer::update(delta);
}

void FloatingButton::visit() {
      CCLayer::visit();
}