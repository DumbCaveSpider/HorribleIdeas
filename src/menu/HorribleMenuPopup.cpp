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

bool HorribleMenuPopup::setup() {
    setID("options"_spr);
    setTitle("Horrible Options");

    auto mainLayerSize = m_mainLayer->getContentSize();

    auto columnLayout = ColumnLayout::create();
    columnLayout->setGap(5.f);
    columnLayout->setAxisReverse(true); // Top to bottom
    columnLayout->setAxisAlignment(AxisAlignment::End);
    columnLayout->setAutoGrowAxis(0.f);

    // Add a background sprite to the popup
    auto optionListBg = CCScale9Sprite::create("square02_001.png");
    optionListBg->setAnchorPoint({ 0.5, 0.5 });
    optionListBg->setPosition({ (mainLayerSize.width / 2.f) - 80.f, (mainLayerSize.height / 2.f) - 12.5f });
    optionListBg->setContentSize({ (mainLayerSize.width / 1.5f) - 25.f, mainLayerSize.height - 45.f });
    optionListBg->setOpacity(50);

    m_mainLayer->addChild(optionListBg);

    // scroll layer
    m_optionList = ScrollLayer::create({ optionListBg->getScaledContentWidth() - 10.f, optionListBg->getScaledContentHeight() - 10.f });
    m_optionList->setID("options-layer");
    m_optionList->setAnchorPoint({ 0.5, 0.5 });
    m_optionList->ignoreAnchorPointForPosition(false);
    m_optionList->setPosition(optionListBg->getPosition());
    m_optionList->m_contentLayer->setLayout(columnLayout);

    m_mainLayer->addChild(m_optionList);

    // add a list button background
    auto filterMenuBg = CCScale9Sprite::create("square02_001.png");
    filterMenuBg->setAnchorPoint({ 0.5, 0.5 });
    filterMenuBg->setPosition({ mainLayerSize.width - 82.5f, (mainLayerSize.height / 2.f) - 12.5f });
    filterMenuBg->setContentSize({ (mainLayerSize.width / 3.f), mainLayerSize.height - 45.f });
    filterMenuBg->setOpacity(50);

    m_mainLayer->addChild(filterMenuBg);

    // filter buttons :o
    auto filterMenu = CCMenu::create();
    filterMenu->setID("filter-menu");
    filterMenu->setAnchorPoint({ 0.5, 1 });
    filterMenu->setPosition({ filterMenuBg->getPositionX(), mainLayerSize.height - 62.5f });

    std::vector<FilterBtnInfo> btns = {
        {SillyTier::Low, "Low", {100, 255, 100}},
        {SillyTier::Medium, "Medium", {255, 255, 100}},
        {SillyTier::High, "High", {255, 100, 100}}
    };

    float btnY = 0.f;
    for (const auto& btn : btns) {
        if (auto normalBs = ButtonSprite::create(btn.label, 0, false, "bigFont.fnt", "GJ_button_01.png", 0, 0.8f)) {
            normalBs->m_label->setColor(btn.color);
            normalBs->setScale(0.8f);

            auto filterBtn = CCMenuItemSpriteExtra::create(normalBs, this, menu_selector(HorribleMenuPopup::filterTierCallback));
            filterBtn->setTag(static_cast<int>(btn.tier));
            filterBtn->setPosition({ 0.f, btnY });
            filterMenu->addChild(filterBtn);

            btnY -= 40.f;
        } else {
            log::error("Failed to create filter button sprite");
        };
    };

    // get the options data
    filterOptionsByTier(options::getAll(), SillyTier::None);

    m_mainLayer->addChild(filterMenu);

    auto modSettingsMenu = CCMenu::create();
    modSettingsMenu->setPosition({ 0.f, 0.f });

    m_mainLayer->addChild(modSettingsMenu);

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
        menu_selector(HorribleMenuPopup::openModSettings)
    );
    modSettingsBtn->setID("mod-settings-button");

    modSettingsMenu->addChild(modSettingsBtn);

    auto seriesBtnSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
    seriesBtnSprite->setScale(0.75f);

    auto seriesBtn = CCMenuItemSpriteExtra::create(
        seriesBtnSprite,
        this,
        menu_selector(HorribleMenuPopup::openSeriesPage)
    );
    seriesBtn->setID("horrible-mods-series-button");
    seriesBtn->setPosition({ mainLayerSize.width - 20.f, mainLayerSize.height - 20.f });

    modSettingsMenu->addChild(seriesBtn);

    // @geode-ignore(unknown-resource)
    auto supporterBtnSprite = CCSprite::createWithSpriteFrameName("geode.loader/gift.png");
    supporterBtnSprite->setScale(0.75f);

    auto supporterBtn = CCMenuItemSpriteExtra::create(
        supporterBtnSprite,
        this,
        menu_selector(HorribleMenuPopup::openSupporterPopup)
    );
    supporterBtn->setID("support-us-button");
    supporterBtn->setPosition({ mainLayerSize.width - 45.f, mainLayerSize.height - 20.f });

    modSettingsMenu->addChild(supporterBtn);

    auto safeModeLabel = CCLabelBMFont::create("Safe Mode: INACTIVE", "bigFont.fnt");
    safeModeLabel->setColor({ 255, 0, 0 });
    safeModeLabel->setAnchorPoint({ 0.5, 0 });
    safeModeLabel->setPosition({ filterMenuBg->getPositionX(), 20.f });
    safeModeLabel->setScale(0.325f);

    // Set safemode label if active
    if (horribleMod->getSettingValue<bool>("safe-mode")) {
        safeModeLabel->setCString("Safe Mode: ACTIVE");
        safeModeLabel->setColor({ 0, 255, 0 });
    } else {
        log::warn("Safe mode is inactive");
    };

    m_mainLayer->addChild(safeModeLabel, 100);

    if (auto optionManager = OptionManager::get()) {
        for (const auto& option : optionManager->getCategories()) {
            log::debug("Registered category: {}", option);
        };
    } else {
        log::error("OptionManager not found");
    };

    return true;
};

void HorribleMenuPopup::filterOptionsByTier(const std::vector<Option>& allOptions, SillyTier tier) {
    if (m_optionList) {
        m_optionList->m_contentLayer->removeAllChildren();

        for (const auto& opt : allOptions) {
            if (tier == SillyTier::None || opt.silly == tier) {
                if (auto modOption = ModOption::create(
                    {
                    m_optionList->m_contentLayer->getScaledContentWidth(),
                    32.f
                    }, opt
                )) m_optionList->m_contentLayer->addChild(modOption);
            };
        };

        m_optionList->m_contentLayer->updateLayout();
        m_optionList->scrollToTop();
    } else {
        log::error("Option list layer not found");
    };
};

void HorribleMenuPopup::filterTierCallback(CCObject* sender) {
    if (auto filterBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)) {
        SillyTier tier = static_cast<SillyTier>(filterBtn->getTag());

        // Toggle: clicking same button disables filter
        if (s_selectedTier == tier) {
            s_selectedTier = SillyTier::None;
        } else {
            s_selectedTier = tier;
        };

        filterOptionsByTier(options::getAll(), s_selectedTier);
    } else {
        log::error("Filter button cast failed");
    };
};

void HorribleMenuPopup::openModSettings(CCObject*) {
    openSettingsPopup(horribleMod);
};

void HorribleMenuPopup::openSeriesPage(CCObject*) {
    createQuickPopup(
        "Horrible Mods",
        "Watch the series '<cr>Horrible Mods</c>' on YouTube?",
        "Cancel",
        "OK",
        [=](bool, bool btn2) {
            if (btn2) web::openLinkInBrowser("https://www.youtube.com/watch?v=Ssl49pNmW_0&list=PL0dsSu2pR5cERnq7gojZTKVRvUwWo2Ohu");
        }
    );
};

void HorribleMenuPopup::openSupporterPopup(CCObject*) {
    openSupportPopup(horribleMod);
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