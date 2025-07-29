#include "ModOption.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

using namespace geode::prelude;

static float s_optionWidth = 125.f;

// Static method to provide all mod options
std::vector<std::tuple<std::string, std::string, std::string, bool>> ModOption::getAllOptions()
{
    // for simple minded: [modID, modName, modDescription, restartRequired]
    return {
        {"mod1", "Mod One", "Description for Mod One.", false},
        {"mod2", "Mod Two", "Description for Mod Two.", true},
        {"mod3", "Mod Three", "Description for Mod Three.", false}};
}

bool ModOption::init(std::string id, std::string name, std::string description, bool restart)
{
    m_modID = id;
    m_modName = name;
    m_modDescription = description;

    m_restartRequired = restart;

    if (!CCMenu::init())
        return false;

    setID(m_modID);

    setContentSize({s_optionWidth, 25.f});
    setAnchorPoint({0, 1});

    // Horizontal layout: [checkbox] [mod name] [info button]
    
    float y = getContentSize().height / 2.f;
    float x = 0.f;

    auto togglerOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    togglerOff->setScale(0.875f);

    auto togglerOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    togglerOn->setScale(0.875f);

    // set the joke option toggler button
    m_toggler = CCMenuItemToggler::create(
        togglerOff,
        togglerOn,
        this,
        menu_selector(ModOption::onToggle)
    );
    m_toggler->setID("toggle");
    m_toggler->setAnchorPoint({0.5f, 0.5f});
    m_toggler->setPosition({x + 12.f, y});
    m_toggler->setScale(0.875f);

    addChild(m_toggler);
    x += 30.f;

    // create joke option name label
    auto nameLabel = CCLabelBMFont::create(
        m_modName.c_str(),
        "bigFont.fnt",
        getContentSize().width,
        kCCTextAlignmentLeft);
    nameLabel->setID("name");
    nameLabel->setLineBreakWithoutSpace(true);
    nameLabel->setAnchorPoint({0.f, 0.5f});
    nameLabel->setPosition({x, y});
    nameLabel->setScale(0.5f);

    addChild(nameLabel);

    // Place info button right after the mod name
    float infoX = x + nameLabel->getScaledContentSize().width + 15.f;

    auto descBtnSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    descBtnSprite->setScale(0.45f);

    // create joke option description button
    auto descBtn = CCMenuItemSpriteExtra::create(
        descBtnSprite,
        this,
        menu_selector(ModOption::onDescription));
    descBtn->setID("info");
    descBtn->setAnchorPoint({0.5f, 0.5f});
    descBtn->setPosition({infoX, y});

    addChild(descBtn);

    return true;
};

void ModOption::setOptionWidth(float width) {
    s_optionWidth = width;
}

void ModOption::onToggle(CCObject *)
{
    if (m_toggler)
        getMod()->setSavedValue(m_modID, m_toggler->isToggled());
    if (m_restartRequired)
        Notification::create("Restart required!", NotificationIcon::Warning, 2.5f)->show();
};

void ModOption::onDescription(CCObject *)
{
    if (auto popup = FLAlertLayer::create(
            m_modName.c_str(),
            m_modDescription.c_str(),
            "OK"))
        popup->show();
};

ModOption *ModOption::create(std::string id, std::string name, std::string description, bool restart)
{
    auto ret = new ModOption();

    if (ret && ret->init(id, name, description, restart))
    {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};