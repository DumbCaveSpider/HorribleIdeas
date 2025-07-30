#include "ModOption.hpp"
#include "../../modified/LevelManager.hpp"
#include "../../modified/LevelManager.hpp"
#include "../../SillyTier.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool ModOption::init(CCSize const &size, std::string id, std::string name, std::string description, SillyTier silly, bool restart)
{

    m_modID = id;
    m_modName = name;
    m_modDescription = description;

    // Set m_modTier based on SillyTier
    switch (silly)
    {
    case SillyTier::None:
        m_modTier = 0; // No silliness
        break;
    case SillyTier::Low:
        m_modTier = 1;
        break;
    case SillyTier::Medium:
        m_modTier = 2;
        break;
    case SillyTier::High:
        m_modTier = 3;
        break;
    default:
        m_modTier = 0;
        break;
    }

    m_restartRequired = restart;

    if (!CCMenu::init())
        return false;

    setID(m_modID);
    setContentSize({size.width, size.height});
    setAnchorPoint({0.5, 1});

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setID("background");
    bg->setScale(0.5);
    bg->setAnchorPoint({0, 0});
    bg->setContentSize({getScaledContentWidth() * 2.f, getScaledContentHeight() * 2.f});
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
    m_toggler->setAnchorPoint({0.5f, 0.5f});
    m_toggler->setPosition({x + 12.f, yCenter});
    m_toggler->setScale(0.875f);

    // Set toggler state based on saved mod option value
    if (m_mod)
    {
        bool saved = m_mod->getSavedValue<bool>(m_modID);
        m_toggler->toggle(saved);
    }

    addChild(m_toggler);

    x += 30.f;

    // name of the joke
    auto nameLabel = CCLabelBMFont::create(
        m_modName.c_str(),
        "bigFont.fnt",
        getContentSize().width - 80.f,
        kCCTextAlignmentLeft);
    nameLabel->setID("name");
    nameLabel->setLineBreakWithoutSpace(true);
    nameLabel->setAnchorPoint({0.f, 0.5f});
    nameLabel->setPosition({x, yCenter});
    nameLabel->setScale(0.4f);

    // Set color based on m_modTier
    switch (m_modTier)
    {
    case 1: // green
        nameLabel->setColor({0, 255, 0});
        break;
    case 2: // yellow
        nameLabel->setColor({255, 255, 0});
        break;
    case 3: // red
        nameLabel->setColor({255, 0, 0});
        break;
    case 0:
    default: // white
        nameLabel->setColor({255, 255, 255});
        break;
    }

    addChild(nameLabel);

    x += nameLabel->getScaledContentSize().width + 15.f;

    auto descBtnSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    descBtnSprite->setScale(0.75f);

    // info button
    auto descBtn = CCMenuItemSpriteExtra::create(
        descBtnSprite,
        this,
        menu_selector(ModOption::onDescription));
    descBtn->setID("info");
    descBtn->setAnchorPoint({0.5f, 0.5f});
    descBtn->setPosition({getContentSize().width - padding - 10.f, yCenter});

    addChild(descBtn);

    return true;
};

void ModOption::saveTogglerState()
{
    if (m_toggler && m_mod)
    {
        m_mod->setSavedValue(m_modID, m_toggler->isToggled());
    }
};

void ModOption::onToggle(CCObject *)
{
    if (m_toggler)
        m_mod->setSavedValue(m_modID, m_toggler->isToggled());
    if (m_restartRequired)
        Notification::create("Restart required!", NotificationIcon::Warning, 2.5f)->show();

    // If grief option is toggled on, call LevelManager::checkAndDownloadGriefLevel
    if (m_modID == "grief" && m_toggler->isToggled())
    {
        LevelManager::DownloadGriefLevel();
    }

    log::info("Option {} now set to {}", m_modName, m_mod->getSavedValue<bool>(m_modID) ? "disabled" : "enabled"); // wtf is it other way around lmao
};

void ModOption::onDescription(CCObject *)
{
    if (auto popup = FLAlertLayer::create(
            m_modName.c_str(),
            m_modDescription.c_str(),
            "OK"))
        popup->show();
};

void ModOption::onExit()
{
    saveTogglerState();
    CCMenu::onExit();
}

std::string ModOption::getModID()
{
    return m_modID;
};

std::string ModOption::getModName()
{
    return m_modName;
};

std::string ModOption::getModDescription()
{
    return m_modDescription;
};

ModOption *ModOption::create(CCSize const &size, std::string id, std::string name, std::string description, SillyTier silly, bool restart)
{
    auto ret = new ModOption();

    if (ret && ret->init(size, id, name, description, silly, restart))
    {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};