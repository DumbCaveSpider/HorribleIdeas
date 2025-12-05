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
      if (!CCLayer::init())
            return false;

      // get the saved position
      float x = horribleMod->getSavedValue<float>("button-x", 100.0f);
      float y = horribleMod->getSavedValue<float>("button-y", 100.0f);

      m_sprite = CircleButtonSprite::createWithSprite(
          "shocksprite.png"_spr,
          0.9f);
      // set initial scale from settings
      m_sprite->setScale(Mod::get()->getSettingValue<float>("floating-button-scale"));
      m_sprite->setOpacity(Mod::get()->getSettingValue<int64_t>("opacity-floating-button"));
      this->setContentSize({m_sprite->getContentSize()});
      this->setAnchorPoint({0, 0});
      this->addChild(m_sprite);

      this->setPosition({x, y});

      // enable touches and schedule updates
      this->setTouchMode(kCCTouchesOneByOne);
      this->setTouchEnabled(true);
      this->setTouchPriority(-512);  // ewww touch priority
      this->setID("floating-button"_spr);
      this->setZOrder(10000);
      this->scheduleUpdate();

      return true;
}

void FloatingButton::setPosition(const CCPoint& position) {
      CCLayer::setPosition(position);
      // save the position
      horribleMod->setSavedValue<float>("button-x", position.x);
      horribleMod->setSavedValue<float>("button-y", position.y);
}

bool FloatingButton::ccTouchBegan(CCTouch* touch, CCEvent* ev) {
      CCPoint touchLocation = touch->getLocation();
      CCPoint nodeLocation = this->getPosition();
      float radius = 30.0f;  // small grab radius for the button

      if (ccpDistance(touchLocation, nodeLocation) <= radius) {
            isDragging = true;
            dragStartPos = ccpSub(nodeLocation, touchLocation);
                  m_sprite->stopAllActions();
                  m_isAnimating = true;
                  m_sprite->runAction(CCSequence::create(
                CCScaleTo::create(0.1f, Mod::get()->getSettingValue<float>("floating-button-scale") - 0.2f),
                CCCallFunc::create(this, callfunc_selector(FloatingButton::onScaleEnd)),
                nullptr));
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
      }
}
void FloatingButton::ccTouchEnded(CCTouch* touch, CCEvent* ev) {
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
            m_sprite->stopAllActions();
            m_isAnimating = true;
            m_sprite->runAction(CCSequence::create(
          CCScaleTo::create(0.1f, Mod::get()->getSettingValue<float>("floating-button-scale")),
          CCCallFunc::create(this, callfunc_selector(FloatingButton::onScaleEnd)),
          nullptr));
}

void FloatingButton::update(float delta) {
      CCLayer::update(delta);
      m_sprite->setOpacity(Mod::get()->getSettingValue<int64_t>("opacity-floating-button"));
      // Avoid overriding scaling when dragging or animating
      float baseScale = Mod::get()->getSettingValue<float>("floating-button-scale");
      if (!isDragging && !m_isAnimating) {
            m_sprite->setScale(baseScale);
      }
      if (Mod::get()->getSettingValue<bool>("floating-button")) {
            this->setVisible(true);
            this->setTouchEnabled(true);
      } else {
            this->setVisible(false);
            this->setTouchEnabled(false);
      }
}

void FloatingButton::visit() {
      CCLayer::visit();
}

void FloatingButton::onScaleEnd() {
      m_isAnimating = false;
}