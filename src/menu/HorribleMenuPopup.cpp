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

// add yo mods here :D
std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>> HorribleMenuPopup::getAllOptions() {
    // for simple minded: [modID, modName, modDescription, sillyTier, restartRequired]
    return {
        {"oxygen",
         "Oxygen Level",
         "Limited oxygen level. You gain oxygen as a flying gamemode. If your oxygen runs out, your player dies.\n<cy>Credit: ArcticWoof</c>",
         SillyTier::High,
         false},
        {"health",
         "Player Health",
         "Add a health bar and decreases everytime you collide with a hazard. If your health reaches zero, your player dies.\n<cy>Credit: Cheeseworks</c>",
         SillyTier::Medium,
         false},
        {"grief",
         "Get Back on Grief",
         "A chance at death of forcing you to play Grief.\n<cy>Credit: Sweep</c>",
         SillyTier::High,
         false},
        {"congregation",
         "Congregation Jumpscare",
         "A chance at death of forcing you to play Congregation.\n<cy>Credit: StaticGD</c>",
         SillyTier::High,
         false},
        {"mock",
         "Mock your 90%+ Fail",
         "Shows a screenshot of one of your 90%-99% fails in the main menu.\n<cy>Credit: Wuffin</c>\n\n<cr>Note: This will not work on macOS and iOS</c>",
         SillyTier::Medium,
         false},
        {"freeze",
         "Random 90%+ FPS Drop",
         "Your visual framerate starts randomly dropping between 90-99% while playing.\n<cy>Credit: Hexfire</c>",
         SillyTier::Medium,
         false},
        {"achieve",
         "Random Achievements",
         "Play the achievement sound when doing random things.\n<cy>Credit: Cheeseworks</c>",
         SillyTier::Low,
         false},
        {"crash_death",
         "Crash Chance on Death",
         "When you die in the level, there's a small chance your game will die too. Don't worry your progress is saved when crashed :)\n<cy>Credit: DragonixGD</c>",
         SillyTier::High,
         false},
        {"math_quiz",
         "Richard's Math Quiz!",
         "When playing a level in practice mode, there's a chance Richard will pop out and give you a quick math quiz. Answer correctly to continue, or restart the level from the beginning.\n<cy>Credit: CyanBoi</c>",
         SillyTier::High,
         false},
        {"no_jump",
         "Randomly Don't Jump",
         "When making an input in a level, there will be a chance your character does not jump.\n<cy>Credit: GilanyKing12</c>",
         SillyTier::Low,
         false},
        {"gravity",
         "Randomize Gravity",
         "Every time you jump in the level, gravity force will increase or decrease randomly.\n<cy>Credit: NJAgain</c>",
         SillyTier::Low,
         false},
        {"death",
         "Fake Death",
         "The player's death effect will play but will not die.\n<cy>Credit: DragonixGD</c>",
         SillyTier::Medium,
         false},
        {"upside_down",
         "Upside-Down Chance",
         "When navigating the game, there's a chance it'll be upside-down and probably break everything.\n<cy>Credit: Cheeseworks</c>",
         SillyTier::Medium,
         false},
        {"ads",
         "Level Ads",
         "While playing a level in normal mode, an ad for a random level will pop up on the screen from time to time.\n<cy>Credit: staticGD</c>",
         SillyTier::Medium,
         false},
        {"black_screen",
         "Black Screen Blink",
         "The screen can suddenly turn black for a split second while playing a level.\n<cy>Credit: elite_smiler_ispro</c>",
         SillyTier::Low,
         false},
        {"parry",
         "Parry Obstacles",
         "Whenever your hitbox is inside of a hazard hitbox, you will not die if you time your click right.\n<cy>Credit: Wuffin</c>",
         SillyTier::Low,
         false},
        {"double_jump",
         "Double-Jump",
         "Allow your character to double-jump in a level.\n<cy>Credit: Cheeseworks</c>",
         SillyTier::Low,
         false},
        {"sleepy",
         "Sleepy Player",
         "Your character will occasionally fall asleep while playing.\n<cy>Credit: this_guy_yt</c>",
         SillyTier::Low,
         false},
        {"pauses",
         "Random Pauses",
         "While playing a level, the game will randomly pause itself.\n<cy>Credit: DragonixGD</c>",
         SillyTier::Low,
         false},
        {"ice_level",
         "Ice Level",
         "Make every surface icy. Slip and slide!\n<cy>Credit: TimeRed</c>",
         SillyTier::Medium,
         false},
        {"random_mirror",
         "Random Mirror Portal",
         "Randomly activates a mirror portal in the level.\n<cy>Credit: TimeRed</c>",
         SillyTier::Low,
         false},
        {"random_speed",
         "Random Speed Change",
         "Randomly changes your speed while playing a level.\n<cy>Credit: imdissapearinghelp</c>",
         SillyTier::Medium,
         false},
        {"random_icon",
         "Random Icon Change",
         "Randomly change your icon every time you jump.\n<cy>Credit: JompyDoJump</c>",
         SillyTier::Low,
         false},
        {"blinking_icon",
         "Blinking Icon",
         "Your icon will start to randomly blink.\n<cy>Credit: DragonixGD</c>",
         SillyTier::Low,
         false} };
};

// silly tier filter
static SillyTier s_selectedTier = SillyTier::None;

void filterOptionsByTier(ScrollLayer* optionsScrollLayer, const std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>>& allOptions, SillyTier tier) {
    optionsScrollLayer->m_contentLayer->removeAllChildren();
    for (const auto& option : allOptions) {
        const auto& [id, name, desc, silly, restart] = option;
        if (tier == SillyTier::None || silly == tier) {
            if (auto modOption = ModOption::create({ optionsScrollLayer->m_contentLayer->getScaledContentWidth(), 32.f }, id, name, desc, silly, restart))
                optionsScrollLayer->m_contentLayer->addChild(modOption);
        }
    }
    optionsScrollLayer->m_contentLayer->updateLayout();
    optionsScrollLayer->scrollToTop();
}

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
        {SillyTier::High, "High", {255, 100, 100}} };

    float btnY = 0.f;
    for (const auto& btn : btns) {
        // Create a ButtonSprite using caption + font overload, then tint its internal label
        auto bs = ButtonSprite::create(btn.label, 0, false, "bigFont.fnt", "GJ_button_01.png", 0, 1.f);
        if (bs && bs->m_label) {
            bs->m_label->setColor(btn.color);
            bs->setScale(1.f);
        };

        auto btnItem = CCMenuItemSpriteExtra::create(bs, this, menu_selector(HorribleMenuPopup::filterTierCallback));
        btnItem->setTag(static_cast<int>(btn.tier));
        btnItem->setPosition({ 0.f, btnY });

        filterMenu->addChild(btnItem);

        btnY -= 40.f;
    };

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
    auto modOptions = getAllOptions();
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

    auto safeModeLabel = CCLabelBMFont::create("!! Safe Mode INACTIVE !!", "chatFont.fnt");
    safeModeLabel->setColor({ 255, 0, 0 });
    safeModeLabel->setAnchorPoint({ 0.5f, 0.0f });
    safeModeLabel->setPosition({ m_mainLayer->getContentSize().width / 2.f, 5.f });
    safeModeLabel->setScale(0.5f);

    // Set safemode label if active
    if (horribleMod->getSettingValue<bool>("safe-mode")) {
        safeModeLabel->setCString("!! Safe Mode ACTIVE !!");
        safeModeLabel->setColor({ 255, 255, 0 });
    } else {
        log::warn("Safe mode is inactive");
    };

    m_mainLayer->addChild(safeModeLabel, 100);
    return true;
}

void HorribleMenuPopup::filterTierCallback(CCObject* sender) {
    auto node = static_cast<CCNode*>(sender);
    SillyTier tier = static_cast<SillyTier>(node->getTag());

    // Toggle: clicking same button disables filter
    if (s_selectedTier == tier) {
        s_selectedTier = SillyTier::None;
    } else {
        s_selectedTier = tier;
    };

    // Find scroll layer and options
    auto optionsScrollLayer = static_cast<ScrollLayer*>(m_mainLayer->getChildByID("scrollLayer"));
    auto modOptions = getAllOptions();

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