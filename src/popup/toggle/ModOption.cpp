#include "ModOption.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

using namespace geode::prelude;

static float s_optionWidth = 125.f;

// add yo mods here :D
std::vector<std::tuple<std::string, std::string, std::string, bool>> ModOption::getAllOptions()
{
    // for simple minded: [modID, modName, modDescription, restartRequired]
    return {
        {"oxygen", "Oxygen Level", "Add an oxygen level if the level has 'Water' in the level name.\n<cy>Credit: ArcticWoof</c>", false},
        {"mock", "Mock your 90% Fail", "Shows a screenshot of your 90% fail everywhere.\n<cy>Credit: Wuffin</c>", true},
        {"freeze", "Random 90% Freeze", "A random chance your game freezes (or fps drops) between 90-99% of the level in normal mode.\n<cy>Credit: Hexfire</c>", false},
        {"grief", "Get Back on Grief", "A random chance of forcing you to play Grief.\n<cy>Credit: Sweep</c>", false}};
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

    // Set the content size and background
    float bgHeight = 32.f;
    setContentSize({s_optionWidth, bgHeight});
    setAnchorPoint({0, 1});

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setContentSize(getContentSize());
    bg->setAnchorPoint({0, 1});
    bg->setOpacity(40);
    bg->setPosition({0, getContentSize().height});
    this->addChild(bg, -1);

    // Horizontal layout: [toggle] [name] [info]
    float padding = 10.f;
    float yCenter = getContentSize().height / 2.f;
    float x = padding;

    auto togglerOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    togglerOff->setScale(0.875f);
    auto togglerOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    togglerOn->setScale(0.875f);
    m_toggler = CCMenuItemToggler::create(
        togglerOff,
        togglerOn,
        this,
        menu_selector(ModOption::onToggle));
    m_toggler->setID("toggle");
    m_toggler->setAnchorPoint({0.5f, 0.5f});
    m_toggler->setPosition({x + 12.f, yCenter});
    m_toggler->setScale(0.875f);
    addChild(m_toggler);
    x += 30.f;

    auto nameLabel = CCLabelBMFont::create(
        m_modName.c_str(),
        "bigFont.fnt",
        getContentSize().width - 80.f,
        kCCTextAlignmentLeft);
    nameLabel->setID("name");
    nameLabel->setLineBreakWithoutSpace(true);
    nameLabel->setAnchorPoint({0.f, 0.5f});
    nameLabel->setPosition({x, yCenter});
    nameLabel->setScale(0.5f);
    addChild(nameLabel);
    x += nameLabel->getScaledContentSize().width + 15.f;

    auto descBtnSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    descBtnSprite->setScale(0.45f);
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

void ModOption::setOptionWidth(float width)
{
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