#include "ModOption.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

using namespace geode::prelude;

bool ModOption::init(std::string id, std::string name, std::string description, bool restart) {
    m_modID = id;
    m_modName = name;
    m_modDescription = description;

    m_restartRequired = restart;

    if (!CCMenu::init()) return false;

    setID(m_modID);
    setContentSize({ 125.f, 25.f });
    setAnchorPoint({ 0, 1 });

    auto togglerOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    togglerOff->setScale(0.875f);

    auto togglerOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    togglerOn->setScale(0.875f);

    m_toggler = CCMenuItemToggler::create(
        togglerOff,
        togglerOn,
        this,
        menu_selector(ModOption::onToggle)
    );
    m_toggler->setID("toggle");
    m_toggler->setAnchorPoint({ 0.5f, 0.5f });
    m_toggler->setPosition({ 7.5f, getScaledContentHeight() / 2.f });
    m_toggler->setScale(0.625f);

    addChild(m_toggler);

    auto nameLabel = CCLabelBMFont::create(
        m_modName.c_str(),
        "bigFont.fnt",
        getScaledContentWidth() - 25.f,
        kCCTextAlignmentLeft
    );
    nameLabel->setID("name");
    nameLabel->setLineBreakWithoutSpace(true);
    nameLabel->setAnchorPoint({ 0.f, 0.5f });
    nameLabel->setPosition({ 20.f, getScaledContentHeight() / 2.f });
    nameLabel->setScale(0.4f);

    addChild(nameLabel);

    auto descBtnSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    descBtnSprite->setScale(0.375f);

    auto descBtn = CCMenuItemSpriteExtra::create(
        descBtnSprite,
        this,
        menu_selector(ModOption::onDescription)
    );
    descBtn->setID("info");
    descBtn->setAnchorPoint({ 0.5f, 0.5f });
    descBtn->setPosition({ getScaledContentWidth() - 2.5f, getScaledContentHeight() - 2.5f });

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
        "OK"
    )) popup->show();
};

ModOption* ModOption::create(std::string id, std::string name, std::string description, bool restart) {
    auto ret = new ModOption();

    if (ret && ret->init(id, name, description, restart)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};