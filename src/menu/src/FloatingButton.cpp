#include "../FloatingButton.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class FloatingButton::Impl final {
public:
    float m_scale = static_cast<float>(horribleMod->getSettingValue<double>("floating-button-scale"));
    int64_t m_opacity = horribleMod->getSettingValue<int64_t>("floating-button-opacity");

    bool m_isDragging = false;
    bool m_isMoving = false;

    CCPoint m_dragStartPos = { 0, 0 };
    Ref<CircleButtonSprite> m_sprite = nullptr;

    bool m_isAnimating = false;
};

FloatingButton::FloatingButton() {
    m_impl = std::make_unique<Impl>();
};

FloatingButton::~FloatingButton() {};

bool FloatingButton::init() {
    if (!CCLayer::init()) return false;

    // get the saved position
    float x = horribleMod->getSavedValue<float>("button-x", 100.f);
    float y = horribleMod->getSavedValue<float>("button-y", 125.f);

    setID("floating-button"_spr);
    setPosition({ x, y });
    setAnchorPoint({ 0.5, 0.5 });
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    setTouchPriority(-512);  // ewww touch priority
    setZOrder(10000);

    m_impl->m_sprite = CircleButtonSprite::createWithSprite(
        "shocksprite.png"_spr,
        0.9f
    );
    m_impl->m_sprite->setAnchorPoint({ 0.5, 0.5 });

    setContentSize(m_impl->m_sprite->getScaledContentSize());

    m_impl->m_sprite->setPosition({ getScaledContentWidth() / 2.f, getScaledContentHeight() / 2.f });

    setScale(m_impl->m_scale); // set initial scale
    setOpacity(m_impl->m_opacity); // set initial opacity

    addChild(m_impl->m_sprite);

    return true;
};

void FloatingButton::setOpacity(GLubyte opacity) {
    if (m_impl->m_sprite) m_impl->m_sprite->setOpacity(opacity);
};

void FloatingButton::setScale(float scale) {
    m_impl->m_scale = scale;

    if (!m_impl->m_isDragging && !m_impl->m_isAnimating) {
        CCLayer::setScale(scale);
        if (m_impl->m_sprite) m_impl->m_sprite->setScale(scale);
    };
};

void FloatingButton::setPosition(const CCPoint& position) {
    CCLayer::setPosition(position);

    // save the position
    if (!m_impl->m_isDragging) {
        horribleMod->setSavedValue<float>("button-x", position.x);
        horribleMod->setSavedValue<float>("button-y", position.y);
    };
};

bool FloatingButton::ccTouchBegan(CCTouch* touch, CCEvent* ev) {
    CCPoint touchLocation = convertToNodeSpace(touch->getLocation());

    auto box = m_impl->m_sprite->boundingBox();
    if (box.containsPoint(touchLocation)) {
        m_impl->m_isDragging = true;

        m_impl->m_dragStartPos = ccpSub(getPosition(), touch->getLocation());

        m_impl->m_sprite->stopAllActions();
        m_impl->m_isAnimating = true;

        m_impl->m_sprite->runAction(CCSequence::create(
            CCScaleTo::create(0.1f, m_impl->m_scale - 0.2f),
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
        CCScaleTo::create(0.1f, m_impl->m_scale),
        CCCallFunc::create(this, callfunc_selector(FloatingButton::onScaleEnd)),
        nullptr));
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

FloatingButton* FloatingButton::get() {
    static auto instance = FloatingButton::create();
    return instance;
};