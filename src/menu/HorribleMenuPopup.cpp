#include "HorribleMenuPopup.hpp"

#include <Geode/Geode.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/terminate.hpp>
#include <Horrible.hpp>

#include "toggle/ModOption.hpp"

using namespace geode::prelude;
using namespace horrible;

class HorribleMenuPopup::Impl final {
public:
    SillyTier s_selectedTier = SillyTier::None;
    bool m_showIncompatible = horribleMod->getSettingValue<bool>("show-incompatible");
    std::string m_searchText = "";

    Ref<ScrollLayer> m_optionList = nullptr;
    Ref<TextInput> m_searchInput = nullptr;
};

HorribleMenuPopup::HorribleMenuPopup() {
    m_impl = std::make_unique<Impl>();
};

HorribleMenuPopup::~HorribleMenuPopup() {};

bool HorribleMenuPopup::setup() {
    setID("options"_spr);
    setTitle("Horrible Options");

    auto mainLayerSize = m_mainLayer->getContentSize();

    auto columnLayout = ColumnLayout::create();
    columnLayout->setGap(5.f);
    columnLayout->setAxisReverse(true);  // Top to bottom
    columnLayout->setAxisAlignment(AxisAlignment::End);
    columnLayout->setAutoGrowAxis(0.f);

    // Add a background sprite to the popup
    auto optionListBg = CCScale9Sprite::create("square02_001.png");
    optionListBg->setAnchorPoint({ 0.5, 0.5 });
    optionListBg->setPosition({ (mainLayerSize.width / 2.f) - 80.f, (mainLayerSize.height / 2.f) - 32.5f });
    optionListBg->setContentSize({ (mainLayerSize.width / 1.5f) - 25.f, mainLayerSize.height - 85.f });
    optionListBg->setOpacity(50);

    m_mainLayer->addChild(optionListBg);

    // scroll layer
    m_impl->m_optionList = ScrollLayer::create({ optionListBg->getScaledContentWidth() - 10.f, optionListBg->getScaledContentHeight() - 10.f });
    m_impl->m_optionList->setID("options-layer");
    m_impl->m_optionList->setAnchorPoint({ 0.5, 0.5 });
    m_impl->m_optionList->ignoreAnchorPointForPosition(false);
    m_impl->m_optionList->setPosition(optionListBg->getPosition());
    m_impl->m_optionList->m_contentLayer->setLayout(columnLayout);

    m_mainLayer->addChild(m_impl->m_optionList);

    // add search bar
    m_impl->m_searchInput = TextInput::create(270, "Search...", "bigFont.fnt");
    m_impl->m_searchInput->setID("search-input");
    m_impl->m_searchInput->setPosition({ optionListBg->getPositionX(), mainLayerSize.height - 52.5f });

    m_impl->m_searchInput->setCallback([this](std::string const& str) {
        m_impl->m_searchText = str;
        filterOptionsByTier(options::getAll(), m_impl->s_selectedTier);  // lets search this crap
                                       });

    m_mainLayer->addChild(m_impl->m_searchInput);

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
        {SillyTier::High, "High", {255, 100, 100}} };

    float btnY = 0.f;
    for (const auto& btn : btns) {
        if (auto normalBs = ButtonSprite::create(btn.label, 125, true, "bigFont.fnt", "GJ_button_01.png", 0, .8f)) {
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
        menu_selector(HorribleMenuPopup::openModSettings));
    modSettingsBtn->setID("mod-settings-button");

    modSettingsMenu->addChild(modSettingsBtn);

    auto seriesBtnSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
    seriesBtnSprite->setScale(0.75f);

    auto seriesBtn = CCMenuItemSpriteExtra::create(
        seriesBtnSprite,
        this,
        menu_selector(HorribleMenuPopup::openSeriesPage));
    seriesBtn->setID("horrible-mods-series-button");
    seriesBtn->setPosition({ mainLayerSize.width - 20.f, mainLayerSize.height - 20.f });

    modSettingsMenu->addChild(seriesBtn);

    // @geode-ignore(unknown-resource)
    auto supporterBtnSprite = CCSprite::createWithSpriteFrameName("geode.loader/gift.png");
    supporterBtnSprite->setScale(0.75f);

    auto supporterBtn = CCMenuItemSpriteExtra::create(
        supporterBtnSprite,
        this,
        menu_selector(HorribleMenuPopup::openSupporterPopup));
    supporterBtn->setID("support-us-button");
    supporterBtn->setPosition({ mainLayerSize.width - 45.f, mainLayerSize.height - 20.f });

    modSettingsMenu->addChild(supporterBtn);

    auto safeModeLabel = CCLabelBMFont::create("Safe Mode OFF", "bigFont.fnt");
    safeModeLabel->setColor({ 255, 0, 0 });
    safeModeLabel->setAnchorPoint({ 0.5, 0 });
    safeModeLabel->setPosition({ filterMenuBg->getPositionX(), 20.f });
    safeModeLabel->setScale(0.325f);

    // Set safemode label if active
    if (horribleMod->getSettingValue<bool>("safe-mode")) {
        safeModeLabel->setCString("Safe Mode ON");
        safeModeLabel->setColor({ 0, 255, 0 });
    } else {
        log::warn("Safe mode is inactive");
    };

    m_mainLayer->addChild(safeModeLabel, 100);

    return true;
};

void HorribleMenuPopup::filterOptionsByTier(const std::vector<Option>& allOptions, SillyTier tier) {
    if (m_impl->m_optionList) {
        m_impl->m_optionList->m_contentLayer->removeAllChildren();

        for (const auto& opt : allOptions) {
            // tier filter
            bool tierMatches = (tier == SillyTier::None || opt.silly == tier);

            // search filter
            bool searchMatches = true;
            if (!m_impl->m_searchText.empty()) {
                std::string searchLower = m_impl->m_searchText;
                std::string nameLower = opt.name;

                // convert lowercase stuff
                searchLower = str::toLower(searchLower);
                nameLower = str::toLower(nameLower);

                searchMatches = str::contains(nameLower, searchLower);
            };

            if (tierMatches && searchMatches) {
                if (auto modOption = ModOption::create(
                    { m_impl->m_optionList->m_contentLayer->getScaledContentWidth(),
                     32.5f },
                    opt)) {
                    if (modOption->isCompatible() || m_impl->m_showIncompatible) {
                        m_impl->m_optionList->m_contentLayer->addChild(modOption);
                    } else {
                        log::error("{} is incompatible with the current platform", modOption->getOption().id);
                        modOption->removeMeAndCleanup();
                    };
                };
            };
        };

        m_impl->m_optionList->m_contentLayer->updateLayout();
        m_impl->m_optionList->scrollToTop();
    } else {
        log::error("Option list layer not found");
    };
};

void HorribleMenuPopup::filterTierCallback(CCObject* sender) {
    if (auto filterBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)) {
        SillyTier tier = static_cast<SillyTier>(filterBtn->getTag());

        // Toggle: clicking same button disables filter
        if (m_impl->s_selectedTier == tier) {
            m_impl->s_selectedTier = SillyTier::None;
        } else {
            m_impl->s_selectedTier = tier;
        };

        filterOptionsByTier(options::getAll(), m_impl->s_selectedTier);
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
        });
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