#include "HorribleMenuPopup.hpp"
#include "toggle/ModOption.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/ui/GeodeUI.hpp>

#include <Geode/utils/terminate.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;
using namespace horrible;

// silly tier filter
static SillyTier s_selectedTier = SillyTier::None;

void filterOptionsByTier(ScrollLayer* optionsScrollLayer, const std::vector<Option>& allOptions, SillyTier tier) {
    optionsScrollLayer->m_contentLayer->removeAllChildren();

    for (const auto& option : allOptions) {
        const auto& [id, name, desc, silly, restart] = option;
        if (tier == SillyTier::None || silly == tier) {
            if (auto modOption = ModOption::create(
                {
                optionsScrollLayer->m_contentLayer->getScaledContentWidth(),
                32.f
                },
                id, name, desc, silly, restart
            )) optionsScrollLayer->m_contentLayer->addChild(modOption);
        };
    };

    optionsScrollLayer->m_contentLayer->updateLayout();
    optionsScrollLayer->scrollToTop();
};

bool HorribleMenuPopup::setup() {
    setID("options"_spr);
    setTitle("Horrible Options");

    auto mainLayerSize = m_mainLayer->getContentSize();

    // Add a background sprite to the popup
    auto optionScrollBg = CCScale9Sprite::create("square02_001.png");
    optionScrollBg->setAnchorPoint({ 0.5, 0.5 });
    optionScrollBg->setPosition({ mainLayerSize.width / 2.f - 80.f, mainLayerSize.height / 2.f - 10.f });
    optionScrollBg->setContentSize({ mainLayerSize.width / 1.5f - 25.f, mainLayerSize.height - 45.f });
    optionScrollBg->setOpacity(50);
    m_mainLayer->addChild(optionScrollBg);

    // add a list button background
    auto listBtnBg = CCScale9Sprite::create("square02_001.png");
    listBtnBg->setAnchorPoint({ 0.5, 0.5 });
    listBtnBg->setPosition({ mainLayerSize.width - (mainLayerSize.width / 2.f - 25.f) + 115.f, mainLayerSize.height / 2.f - 10.f });
    listBtnBg->setContentSize({ mainLayerSize.width / 3.f, mainLayerSize.height - 45.f });
    listBtnBg->setOpacity(50);
    m_mainLayer->addChild(listBtnBg);

    // filter buttons :o
    auto filterMenu = CCMenu::create();
    filterMenu->setPosition({ mainLayerSize.width - 85.f, mainLayerSize.height / 2.f + 80.f });

    struct FilterBtnInfo {
        SillyTier tier;
        const char* label;
        ccColor3B color;
    };

    std::vector<FilterBtnInfo> btns = {
        {SillyTier::Low, "Low", {100, 255, 100}},
        {SillyTier::Medium, "Medium", {255, 255, 100}},
        {SillyTier::High, "High", {255, 100, 100}}
    };

    float btnY = 0.f;
    for (const auto& btn : btns) {
        auto normalBS = ButtonSprite::create(btn.label, 0, false, "bigFont.fnt", "GJ_button_01.png", 0, 0.8f);
        if (normalBS && normalBS->m_label) {
            normalBS->m_label->setColor(btn.color);
            normalBS->setScale(.8f);
        }

        auto filterBtn = CCMenuItemSpriteExtra::create(normalBS, this, menu_selector(HorribleMenuPopup::filterTierCallback));
        filterBtn->setTag(static_cast<int>(btn.tier));
        filterBtn->setPosition({ 0.f, btnY });
        filterMenu->addChild(filterBtn);
        btnY -= 40.f;
    }

    m_mainLayer->addChild(filterMenu);

    auto columnLayout = ColumnLayout::create();
    columnLayout->setGap(5.f);
    columnLayout->setAxisReverse(true); // Top to bottom
    columnLayout->setAxisAlignment(AxisAlignment::End);
    columnLayout->setAutoGrowAxis(0.f);

    // scroll layer
    auto optionsScrollLayer = ScrollLayer::create({ optionScrollBg->getContentSize().width - 10.f, optionScrollBg->getContentSize().height - 10.f });
    optionsScrollLayer->setID("scrollLayer");
    optionsScrollLayer->setAnchorPoint({ 0.5, 0.5 });
    optionsScrollLayer->ignoreAnchorPointForPosition(false);
    optionsScrollLayer->setPosition(optionScrollBg->getPosition());
    optionsScrollLayer->m_contentLayer->setLayout(columnLayout);

    // get the options data
    filterOptionsByTier(optionsScrollLayer, modOptions, SillyTier::None);

    m_mainLayer->addChild(optionsScrollLayer);

    auto modSettingsMenu = CCMenu::create();
    // add a mod settings at the bottom left
    auto modSettingsBtnSprite = CircleButtonSprite::createWithSpriteFrameName(
        // @geode-ignore(unknown-resource)
        "geode.loader/settings.png",
        1.f,
        CircleBaseColor::Green,
        CircleBaseSize::Medium);
    modSettingsBtnSprite->setScale(0.75f);
    auto modSettingsBtn = CCMenuItemSpriteExtra::create(
        modSettingsBtnSprite,
        this,
        menu_selector(HorribleMenuPopup::openModSettings));
    modSettingsMenu->setPosition({ 0.f, 0.f });
    modSettingsMenu->addChild(modSettingsBtn);
    m_mainLayer->addChild(modSettingsMenu);

    auto safeModeLabel = CCLabelBMFont::create("Safe Mode: INACTIVE", "bigFont.fnt");
    safeModeLabel->setColor({ 255, 0, 0 });
    safeModeLabel->setAnchorPoint({ 0.5f, 0.0f });
    safeModeLabel->setPosition({ mainLayerSize.width - (mainLayerSize.width / 2.f - 25.f) + 115.f, 20.f });
    safeModeLabel->setScale(0.325f);

    // Set safemode label if active
    if (horribleMod->getSettingValue<bool>("safe-mode")) {
        safeModeLabel->setCString("Safe Mode: ACTIVE");
        safeModeLabel->setColor({ 0, 255, 0 });
    } else {
        log::warn("Safe mode is inactive");
    };

    m_mainLayer->addChild(safeModeLabel, 100);
    return true;
}

void HorribleMenuPopup::filterTierCallback(CCObject* sender) {
    auto filterBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
    if (!filterBtn)
        return;
    SillyTier tier = static_cast<SillyTier>(filterBtn->getTag());

    // Toggle: clicking same button disables filter
    if (s_selectedTier == tier) {
        s_selectedTier = SillyTier::None;
    } else {
        s_selectedTier = tier;
    }

    // Find scroll layer and options
    auto optionsScrollLayer = static_cast<ScrollLayer*>(m_mainLayer->getChildByID("scrollLayer"));

    filterOptionsByTier(optionsScrollLayer, modOptions, s_selectedTier);
};

void HorribleMenuPopup::openModSettings(CCObject* sender) {
    openSettingsPopup(horribleMod);
};

HorribleMenuPopup* HorribleMenuPopup::create() {
    auto ret = new HorribleMenuPopup();

    if (ret && ret->initAnchored(450.f, 280.f)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};