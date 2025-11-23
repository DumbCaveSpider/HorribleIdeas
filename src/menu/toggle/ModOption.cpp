#include "ModOption.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

bool ModOption::init(CCSize const& size, Option option) {
    m_option = option;

    // check for compatibility
    for (auto p : m_option.platforms) {
        if (p & GEODE_PLATFORM_TARGET) { s_compatible = true; break; };
    };

    if (!CCMenu::init()) return false;

    setID(m_option.id);
    setScaledContentSize(size);
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

    // Set toggler state based on saved mod option value
    if (horribleMod) m_toggler->toggle(horribleMod->getSavedValue<bool>(m_option.id));

    addChild(m_toggler);

    x += 30.f;

    // name of the joke
    auto nameLabel = CCLabelBMFont::create(
        m_option.name.c_str(),
        "bigFont.fnt",
        getScaledContentSize().width - 80.f,
        kCCTextAlignmentLeft
    );
    nameLabel->setID("name");
    nameLabel->setLineBreakWithoutSpace(true);
    nameLabel->setAnchorPoint({ 0.f, 0.5f });
    nameLabel->setPosition({ x, yCenter });
    nameLabel->setScale(0.4f);

    auto categoryLabel = CCLabelBMFont::create(
        m_option.category.c_str(),
        "goldFont.fnt",
        getScaledContentSize().width - 80.f,
        kCCTextAlignmentLeft
    );
    categoryLabel->setID("category");
    categoryLabel->setLineBreakWithoutSpace(true);
    categoryLabel->setAnchorPoint({ 0.f, 0.5f });
    categoryLabel->setPosition({ x, yCenter + 10.f });
    categoryLabel->setScale(0.25f);

    // Set color based on m_option.Tier
    switch (m_option.silly) {
    case SillyTier::Low: // green
        nameLabel->setColor({ 100, 255, 100 });
        break;

    case SillyTier::Medium: // yellow
        nameLabel->setColor({ 255, 255, 100 });
        break;

    case SillyTier::High: // red
        nameLabel->setColor({ 255, 100, 100 });
        break;

    default: // white
        nameLabel->setColor({ 255, 255, 255 });
        break;
    };

    addChild(nameLabel);
    addChild(categoryLabel);

    if (horribleMod->getSettingValue<bool>("dev-mode")) {
        auto idLabel = CCLabelBMFont::create(
            m_option.id.c_str(),
            "chatFont.fnt",
            getContentSize().width - 20.f,
            kCCTextAlignmentLeft
        );
        idLabel->setID("id");
        idLabel->setLineBreakWithoutSpace(true);
        idLabel->setPosition({ x, yCenter - 10.f });
        idLabel->setAnchorPoint({ 0.f, 0.5f });
        idLabel->setColor({ 0, 0, 0 });
        idLabel->setOpacity(200);
        idLabel->setScale(0.5f);

        addChild(idLabel);
    };

    x += nameLabel->getScaledContentSize().width + 15.f;

    auto descBtnSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    descBtnSprite->setScale(0.75f);

    // info button
    auto descBtn = CCMenuItemSpriteExtra::create(
        descBtnSprite,
        this,
        menu_selector(ModOption::onDescription));
    descBtn->setID("info");
    descBtn->setAnchorPoint({ 0.5f, 0.5f });
    descBtn->setPosition({ getContentSize().width - padding - 10.f, yCenter });

    addChild(descBtn);

    if (!s_compatible) {
        m_toggler->toggle(false);
        m_toggler->setEnabled(false);

        saveTogglerState();

        togglerOff->setColor({ 150, 150, 150 });
        togglerOn->setColor({ 150, 150, 150 });

        bg->setColor({ 150, 150, 150 });

        nameLabel->setColor({ 150, 150, 150 });
        categoryLabel->setColor({ 150, 150, 150 });
    };

    return true;
};

void ModOption::saveTogglerState() {
    if (m_toggler) options::set(m_option.id, m_toggler->isToggled());
};

void ModOption::onToggle(CCObject*) {
    if (m_toggler) {
        auto toggle = m_toggler->isToggled();

        options::set(m_option.id, toggle);
        if (m_option.restart) Notification::create("Restart required!", NotificationIcon::Warning, 2.5f)->show();
    };

    log::info("Option {} now set to {}", m_option.name, options::get(m_option.id) ? "disabled" : "enabled"); // wtf is it other way around lmao
};

void ModOption::onDescription(CCObject*) {
    if (auto popup = FLAlertLayer::create(
        m_option.name.c_str(),
        m_option.description.c_str(),
        "OK"))
        popup->show();
};

void ModOption::onExit() {
    saveTogglerState();
    CCMenu::onExit();
}

std::string ModOption::getModID() {
    return m_option.id;
};

std::string ModOption::getModName() {
    return m_option.name;
};

std::string ModOption::getModDescription() {
    return m_option.description;
};

ModOption* ModOption::create(CCSize const& size, Option option) {
    auto ret = new ModOption();

    if (ret && ret->init(size, option)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
}; 