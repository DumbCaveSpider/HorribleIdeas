#include "HorribleMenuPopup.hpp"

#include "../SillyTier.hpp"
#include "toggle/ModOption.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// add yo mods here :D
std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>> HorribleMenuPopup::getAllOptions() {
    // for simple minded: [modID, modName, modDescription, sillyTier, restartRequired]
    return {
        {
            "oxygen",
            "Oxygen Level",
            "Add an oxygen level. You gain oxygen whenever you are a flying gamemode.\n<cy>Credit: ArcticWoof</c>",
            SillyTier::High,
            false
        },
        {
            "grief",
            "Get Back on Grief",
            "A 10% chance of forcing you to play Grief.\n<cy>Credit: Sweep</c>",
            SillyTier::High,
            false
        },
         {
            "congregation",
            "Congregation Jumpscare",
            "A 10% chance of forcing you to play the Congregation Jumpscare.\n<cy>Credit: StaticGD</c>",
            SillyTier::High,
            false
        },
        {
            "mock",
            "Mock your 90%+ Fail",
            "Shows a screenshot of your 90%-99% fail everywhere.\n<cy>Credit: Wuffin</c>\n<cr>Note: This will not work on macOS and iOS</c>",
            SillyTier::Medium,
            false
        },
        {
            "freeze",
            "Random 90%+ Freeze",
            "A random chance your game freezes (or fps drops) between 90-99% of the level in normal mode.\n<cy>Credit: Hexfire</c>",
            SillyTier::Low,
            false
        },
        {
            "achieve",
            "Random Achievements",
            "Play the achievement sound when doing random things.\n<cy>Credit: Cheeseworks</c>",
            SillyTier::Low,
            false
        },
        {
            "crash-death",
            "Crash Chance on Death",
            "When your character dies in a level, there's a small chance your game will die too.\n<cy>Credit: DragonixGD</c>",
            SillyTier::High,
            false
        },
        {
            "no-jump",
            "Randomly Don't Jump",
            "When making an input in a level, there will be a chance your character does not jump.\n<cy>Credit: GilanyKing12</c>",
            SillyTier::Low,
            false
        },
        {
            "gravity",
            "Randomize Gravity",
            "Every time you jump in the level, the gravity will change to a random value.\n<cy>Credit: NJAgain</c>",
            SillyTier::Low,
            false
        },
        {
            "death",
            "Fake Death",
            "The player's death effect will play but will not die.\n<cy>Credit: DragonixGD</c>",
            SillyTier::Medium,
            false
        },
        {
            "upside-down",
            "Upside-Down Layer Chance",
            "When switching layers, there's a 50/50 chance it'll be upside-down.\n<cy>Credit: Cheeseworks</c>",
            SillyTier::Low,
            false
        }
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

    m_mainLayer->addChild(optionsScrollLayer);

    return true;
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