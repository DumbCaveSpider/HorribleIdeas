#include "../CategoryItem.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

CategoryEvent::CategoryEvent(std::string id) : m_id(id) {};

std::string CategoryEvent::getId() const {
    return m_id;
};

ListenerResult CategoryEventFilter::handle(std::function<Callback> fn, CategoryEvent* event) {
    return fn(event);
};

class CategoryItem::Impl final {
public:
    std::string m_category = ""; // The category name

    Ref<CCMenuItemSpriteExtra> m_toggler = nullptr; // The toggler for the option
    bool m_toggled = false;
};

CategoryItem::CategoryItem() {
    m_impl = std::make_unique<Impl>();
};

CategoryItem::~CategoryItem() {};

bool CategoryItem::init(CCSize const& size, const std::string& category) {
    m_impl->m_category = category;

    if (!CCMenu::init()) return false;

    setID(str::join(str::split(str::toLower(category), " "), "-"));
    setScaledContentSize(size);
    setAnchorPoint({ 0.5, 1 });

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setID("background");
    bg->setScale(0.2f);
    bg->setAnchorPoint({ 0, 0 });
    bg->setContentSize({ getScaledContentWidth() * 5.f, getScaledContentHeight() * 5.f });
    bg->setOpacity(40);

    addChild(bg, -1);

    auto togglerSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    togglerSprite->setScale(0.5f);

    // toggler for the category
    m_impl->m_toggler = CCMenuItemSpriteExtra::create(
        togglerSprite,
        this,
        menu_selector(CategoryItem::onToggle)
    );
    m_impl->m_toggler->setID("toggler");
    m_impl->m_toggler->setAnchorPoint({ 0.5f, 0.5f });
    m_impl->m_toggler->setPosition({ 10.f, getScaledContentHeight() / 2.f });
    m_impl->m_toggler->setScale(0.875f);

    addChild(m_impl->m_toggler);

    // name of the joke
    auto nameLabel = CCLabelBMFont::create(
        m_impl->m_category.c_str(),
        "goldFont.fnt",
        getScaledContentWidth() - 37.5f,
        kCCTextAlignmentLeft
    );
    nameLabel->setID("name-label");
    nameLabel->setLineBreakWithoutSpace(true);
    nameLabel->setAnchorPoint({ 0.f, 0.5f });
    nameLabel->setPosition({ 17.5f, getScaledContentHeight() / 2.f });
    nameLabel->setScale(0.375f);

    addChild(nameLabel);

    return true;
};

void CategoryItem::setButtonSprite(CCMenuItemSpriteExtra* button, const char* frameName) {
    if (button) {
        if (auto sprite = typeinfo_cast<CCSprite*>(button->getNormalImage())) {
            if (auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName)) sprite->setDisplayFrame(frame);
        };
    };
};

ListenerResult CategoryItem::OnCategory(const std::string& category) {
    m_impl->m_toggled = category == m_impl->m_category;

    if (m_impl->m_toggler) {
        auto spr = m_impl->m_toggler->getNormalImage();
        auto toggleSpr = CCSprite::createWithSpriteFrameName(
            m_impl->m_toggled ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png"
        );

        if (toggleSpr && spr) {
            toggleSpr->setScale(spr->getScale());
            toggleSpr->setPosition(spr->getPosition());

            m_impl->m_toggler->setNormalImage(toggleSpr);
        };
    };

    return ListenerResult::Propagate;
};

void CategoryItem::onToggle(CCObject*) {
    CategoryEvent(m_impl->m_toggled ? "" : m_impl->m_category).post();
};

CategoryItem* CategoryItem::create(CCSize const& size, const std::string& category) {
    auto ret = new CategoryItem();
    if (ret && ret->init(size, category)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};