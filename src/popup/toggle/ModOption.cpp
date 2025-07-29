#include "ModOption.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

using namespace geode::prelude;

bool ModOption::init(CCSize const& size, std::string id, std::string name, std::string description, SillyTier silly, bool restart) {
    m_modID = id;
    m_modName = name;
    m_modDescription = description;

    m_restartRequired = restart;

    if (!CCMenu::init()) return false;

    setID(m_modID);
    setContentSize({ size.width, size.height });
    setAnchorPoint({ 0.5, 1 });

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setID("background");
    bg->setScale(0.5);
    bg->setAnchorPoint({ 0, 0 });
    bg->setContentSize({ getScaledContentWidth() * 2.f, getScaledContentHeight() * 2.f });
    bg->setOpacity(40);

    addChild(bg, -1);

    // Horizontal layout: [toggle] [name] [info]
    float padding = 5.f;
    float yCenter = getContentSize().height / 2.f;

    float x = padding;

    auto togglerOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    togglerOff->setScale(0.875f);

    auto togglerOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    togglerOn->setScale(0.875f);

    // toggler for the joke
    m_toggler = CCMenuItemToggler::create(
        togglerOff,
        togglerOn,
        this,
        menu_selector(ModOption::onToggle));
    m_toggler->setID("toggle");
    m_toggler->setAnchorPoint({ 0.5f, 0.5f });
    m_toggler->setPosition({ x + 12.f, yCenter });
    m_toggler->setScale(0.875f);

    addChild(m_toggler);

    x += 30.f;

    // name of the joke 
    auto nameLabel = CCLabelBMFont::create(
        m_modName.c_str(),
        "bigFont.fnt",
        getContentSize().width - 80.f,
        kCCTextAlignmentLeft
    );
    nameLabel->setID("name");
    nameLabel->setLineBreakWithoutSpace(true);
    nameLabel->setAnchorPoint({ 0.f, 0.5f });
    nameLabel->setPosition({ x, yCenter });
    nameLabel->setScale(0.5f);

    addChild(nameLabel);

    x += nameLabel->getScaledContentSize().width + 15.f;

    auto descBtnSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    descBtnSprite->setScale(0.75f);

    // info button
    auto descBtn = CCMenuItemSpriteExtra::create(
        descBtnSprite,
        this,
        menu_selector(ModOption::onDescription)
    );
    descBtn->setID("info");
    descBtn->setAnchorPoint({ 0.5f, 0.5f });
    descBtn->setPosition({ getContentSize().width - padding - 10.f, yCenter });

    addChild(descBtn);

    return true;
};

void ModOption::onToggle(CCObject*) {
    if (m_toggler) getMod()->setSavedValue(m_modID, m_toggler->isToggled());
    if (m_restartRequired) Notification::create("Restart required!", NotificationIcon::Warning, 2.5f)->show();
};

void ModOption::onDescription(CCObject*) {
    if (auto popup = FLAlertLayer::create(
        m_modName.c_str(),
        m_modDescription.c_str(),
        "OK"))
        popup->show();
};

std::string ModOption::getModID() {
    return m_modID;
};

std::string ModOption::getModName() {
    return m_modName;
};

std::string ModOption::getModDescription() {
    return m_modDescription;
};

ModOption* ModOption::create(CCSize const& size, std::string id, std::string name, std::string description, SillyTier silly, bool restart) {
    auto ret = new ModOption();

    if (ret && ret->init(size, id, name, description, silly, restart)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};