#include "HorribleMenuPopup.hpp"
#include "toggle/ModOption.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/ui/GeodeUI.hpp>

#include <Geode/utils/terminate.hpp>

using namespace geode::prelude;
using namespace horrible;

// add yo mods here :D
std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>> HorribleMenuPopup::getAllOptions() {
    // for simple minded: [modID, modName, modDescription, sillyTier, restartRequired]
    return {
        {"oxygen",
         "Oxygen Level",
         "Add an oxygen level. You gain oxygen whenever you are a flying gamemode.\n<cy>Credit: ArcticWoof</c>",
         SillyTier::High,
         false},
        {"health",
         "Player Health",
         "Add a health bar and decreases everytime you taken damage. When your health reaches zero, you die.\n<cy>Credit: Cheeseworks</c>",
         SillyTier::Medium,
         false},
        {"grief",
         "Get Back on Grief",
         "A chance of forcing you to play Grief.\n<cy>Credit: Sweep</c>",
         SillyTier::High,
         false},
        {"congregation",
         "Congregation Jumpscare",
         "A chance of forcing you to play the Congregation Jumpscare.\n<cy>Credit: StaticGD</c>",
         SillyTier::High,
         false},
        {"mock",
         "Mock your 90%+ Fail",
         "Shows a screenshot of one of your 90%-99% fails in the main menu.\n<cy>Credit: Wuffin</c>\n\n<cr>Note: This will not work on macOS and iOS</c>",
         SillyTier::Medium,
         false},
        {"freeze",
         "Random 90%+ FPS Drop",
         "Your game FPS starts dropping between 90-99% while playing.\n<cy>Credit: Hexfire</c>",
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
         "When playing a level in practice mode, there's a chance Richard will pop out and give you a quick math quiz. Answer correctly to continue, or restart from the beginning.\n<cy>Credit: CyanBoi</c>",
         SillyTier::High,
         false},
        {"no_jump",
         "Randomly Don't Jump",
         "When making an input in a level, there will be a chance your character does not jump.\n<cy>Credit: GilanyKing12</c>\n<cr>Note: This will not work in platformer mode</c>",
         SillyTier::Low,
         false},
        {"gravity",
         "Randomize Gravity",
         "Every time you jump in the level, the gravity will change to a random value.\n<cy>Credit: NJAgain</c>\n<cr>Note: This will not work in platformer mode</c>",
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
         "The screen can suddenly turn black for a moment while playing a level.\n<cy>Credit: elite_smiler_ispro</c>",
         SillyTier::Low,
         false},
        {"parry",
         "Parry Obstacles",
         "Whenever your hitbox is inside of an obstacle hitbox, if you time your click right, you don't die.\n<cy>Credit: Wuffin</c>",
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
         "While playing a level, the game will randomly pause the game.\n<cy>Credit: DragonixGD</c>",
         SillyTier::Low,
         false}
    };
};

bool HorribleMenuPopup::setup() {
    setID("options"_spr);
    setTitle("Horrible Options");

    auto mainLayerSize = m_mainLayer->getContentSize();

    // Add a background sprite to the popup
    auto optionScrollBg = CCScale9Sprite::create("square02_001.png");
    optionScrollBg->setAnchorPoint({ 0.5, 0.5 });
    optionScrollBg->setPosition({ mainLayerSize.width / 2.f, mainLayerSize.height / 2.f - 10.f });
    optionScrollBg->setContentSize({ mainLayerSize.width - 25.f, mainLayerSize.height - 45.f });
    optionScrollBg->setOpacity(50);

    m_mainLayer->addChild(optionScrollBg);

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

    // Sort mod options alphabetically by name
    std::sort(modOptions.begin(), modOptions.end(), [](const auto& a, const auto& b) { return std::get<4>(a) < std::get<4>(b); });

    for (const auto& option : modOptions) {
        const auto& [id, name, desc, silly, restart] = option;

        if (auto modOption = ModOption::create({ optionsScrollLayer->m_contentLayer->getScaledContentWidth(), 32.f }, id, name, desc, silly, restart))
            optionsScrollLayer->m_contentLayer->addChild(modOption);
    };

    optionsScrollLayer->m_contentLayer->updateLayout();
    optionsScrollLayer->scrollToTop();

    m_mainLayer->addChild(optionsScrollLayer);

    auto modSettingsMenu = CCMenu::create();

    // add a mod settings at the bottom left
    auto modSettingsBtnSprite = CircleButtonSprite::createWithSpriteFrameName(
        // @geode-ignore(unknown-resource)
        "geode.loader/settings.png",
        1.f,
        CircleBaseColor::Green,
        CircleBaseSize::Medium
    );
    modSettingsBtnSprite->setScale(0.75f);

    auto modSettingsBtn = CCMenuItemSpriteExtra::create(
        modSettingsBtnSprite,
        this,
        menu_selector(HorribleMenuPopup::openModSettings)
    );
    modSettingsMenu->setPosition({ 0.f, 0.f });

    modSettingsMenu->addChild(modSettingsBtn);

    m_mainLayer->addChild(modSettingsMenu);

    auto safeModeLabel = CCLabelBMFont::create("!! Safe Mode INACTIVE !!", "chatFont.fnt");
    safeModeLabel->setColor({ 255, 0, 0 });
    safeModeLabel->setAnchorPoint({ 0.5f, 0.0f });
    safeModeLabel->setPosition({ m_mainLayer->getContentSize().width / 2.f, 5.f });
    safeModeLabel->setScale(0.5f);

    // Set safemode label if active
    if (auto safeMode = getMod()->getSettingValue<bool>("safe-mode")) {
        safeModeLabel->setCString("!! Safe Mode ACTIVE !!");
        safeModeLabel->setColor({ 255, 255, 0 });
    } else {
        log::warn("Safe mode is inactive");
    };

    m_mainLayer->addChild(safeModeLabel, 100);

    return true;
};

void HorribleMenuPopup::openModSettings(CCObject* sender) {
    openSettingsPopup(getMod());
};

HorribleMenuPopup* HorribleMenuPopup::create() {
    auto ret = new HorribleMenuPopup();

    if (ret && ret->initAnchored(300.f, 280.f)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};