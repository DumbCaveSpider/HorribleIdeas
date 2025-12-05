#include "../FloatingButton.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class FloatingButton::Impl final {
public:
    bool m_isDragging = false;
    bool m_isMoving = false;

    CCPoint m_dragStartPos = { 0, 0 };
    CircleButtonSprite* m_sprite = nullptr;

    bool m_isAnimating = false;
};

FloatingButton::FloatingButton() {
    m_impl = std::make_unique<Impl>();
};

FloatingButton::~FloatingButton() {};

FloatingButton* FloatingButton::get() {
    static auto instance = new FloatingButton();
    return instance;
};

bool FloatingButton::init() {
    if (!CCLayer::init()) return false;

    // get the saved position
    float x = horribleMod->getSavedValue<float>("button-x", 100.0f);
    float y = horribleMod->getSavedValue<float>("button-y", 100.0f);

    m_impl->m_sprite = CircleButtonSprite::createWithSprite(
        "shocksprite.png"_spr,
        0.9f
    );
    // set initial scale from settings
    m_impl->m_sprite->setScale(horribleMod->getSettingValue<float>("floating-button-scale"));
    m_impl->m_sprite->setOpacity(horribleMod->getSettingValue<int64_t>("opacity-floating-button"));

    setContentSize({ m_impl->m_sprite->getContentSize() });
    setAnchorPoint({ 0, 0 });

    addChild(m_impl->m_sprite);

    setPosition({ x, y });

    // enable touches and schedule updates
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    setTouchPriority(-512);  // ewww touch priority
    setID("floating-button"_spr);
    setZOrder(10000);
    scheduleUpdate();

    return true;
};

void FloatingButton::setPosition(const CCPoint& position) {
    CCLayer::setPosition(position);
    // save the position
    horribleMod->setSavedValue<float>("button-x", position.x);
    horribleMod->setSavedValue<float>("button-y", position.y);
};

bool FloatingButton::ccTouchBegan(CCTouch* touch, CCEvent* ev) {
    CCPoint touchLocation = touch->getLocation();
    CCPoint nodeLocation = getPosition();

    float radius = 30.0f;  // small grab radius for the button

    if (ccpDistance(touchLocation, nodeLocation) <= radius) {
        m_impl->m_isDragging = true;

        m_impl->m_dragStartPos = ccpSub(nodeLocation, touchLocation);
        m_impl->m_sprite->stopAllActions();

        m_impl->m_isAnimating = true;

        m_impl->m_sprite->runAction(CCSequence::create(
            CCScaleTo::create(0.1f, horribleMod->getSettingValue<float>("floating-button-scale") - 0.2f),
            CCCallFunc::create(this, callfunc_selector(FloatingButton::onScaleEnd)),
            nullptr));
        return true;  // swallow touch
    };

    return false;
};

void FloatingButton::ccTouchMoved(CCTouch* touch, CCEvent* ev) {
    if (m_impl->m_isDragging) {
        CCPoint touchLocation = touch->getLocation();
        CCPoint newLocation = ccpAdd(touchLocation, m_impl->m_dragStartPos);

        setPosition(newLocation);

        m_impl->m_isMoving = true;
    };
};

void FloatingButton::ccTouchEnded(CCTouch* touch, CCEvent* ev) {
    if (!m_impl->m_isMoving) menu::open();

    // reset state
    m_impl->m_isDragging = false;
    m_impl->m_isMoving = false;

    // store position
    horribleMod->setSavedValue<float>("button-x", getPosition().x);
    horribleMod->setSavedValue<float>("button-y", getPosition().y);

    // reset scale
    m_impl->m_sprite->stopAllActions();
    m_impl->m_isAnimating = true;
    m_impl->m_sprite->runAction(CCSequence::create(
        CCScaleTo::create(0.1f, horribleMod->getSettingValue<float>("floating-button-scale")),
        CCCallFunc::create(this, callfunc_selector(FloatingButton::onScaleEnd)),
        nullptr));
};

void FloatingButton::update(float dt) {
    CCLayer::update(dt);
    m_impl->m_sprite->setOpacity(horribleMod->getSettingValue<int64_t>("opacity-floating-button"));
    // Avoid overriding scaling when dragging or animating
    float baseScale = horribleMod->getSettingValue<float>("floating-button-scale");

    if (!m_impl->m_isDragging && !m_impl->m_isAnimating) {
        m_impl->m_sprite->setScale(baseScale);
    };

    if (horribleMod->getSettingValue<bool>("floating-button")) {
        setVisible(true);
        setTouchEnabled(true);
    } else {
        setVisible(false);
        setTouchEnabled(false);
    };
};

void FloatingButton::visit() {
    CCLayer::visit();
};

void FloatingButton::onScaleEnd() {
    m_impl->m_isAnimating = false;
};

FloatingButton* FloatingButton::create() {
    auto ret = new FloatingButton();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};