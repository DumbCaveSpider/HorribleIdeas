#include "HorriblePopupSelect.hpp"

#include "toggle/ModOption.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool HorriblePopupSelect::setup()
{
    setID("options"_spr);
    setTitle("Horrible Options");

    auto mainLayerSize = m_mainLayer->getContentSize();

    // Add a background sprite to the popup
    auto OptionScollBg = CCScale9Sprite::create("square02_001.png");
    OptionScollBg->setAnchorPoint({0.5f, 0.5f});
    OptionScollBg->setPosition(mainLayerSize.width / 2, mainLayerSize.height / 2 - 10.f);
    OptionScollBg->setContentSize({mainLayerSize.width - 25.f, mainLayerSize.height - 45.f});
    OptionScollBg->setOpacity(50);
    m_mainLayer->addChild(OptionScollBg);

    // scroll layer
    auto OptionsScrollLayer = ScrollLayer::create(CCSize(OptionScollBg->getContentSize().width, OptionScollBg->getContentSize().height));
    OptionsScrollLayer->setID("scrollLayer");
    OptionsScrollLayer->setScale(0.95f);
    OptionsScrollLayer->setAnchorPoint({0.5f, 0.5f});
    OptionsScrollLayer->setPosition({13.f, 13.f});
    m_mainLayer->addChild(OptionsScrollLayer);

    // Create a ColumnLayout for the mod options
    auto optionsColumn = ColumnLayout::create()
        ->setAxisAlignment(AxisAlignment::End)
        ->setCrossAxisAlignment(AxisAlignment::Center)
        ->setAutoGrowAxis(0.95f)
        ->setGap(10.f);

    auto optionsContent = OptionsScrollLayer->m_contentLayer;

    std::vector<std::tuple<std::string, std::string, std::string, bool>> modOptions = {
        {"mod1", "Mod One", "Description for Mod One.", false},
        {"mod2", "Mod Two", "Description for Mod Two.", true},
        {"mod3", "Mod Three", "Description for Mod Three.", false}};

    for (const auto &option : modOptions)
    {
        const auto &[id, name, desc, enabled] = option;
        auto modOption = ModOption::create(id, name, desc, enabled);
        if (modOption != nullptr)
        {
            optionsContent->addChild(modOption);
        }
    }


    return true;
};

HorriblePopupSelect *HorriblePopupSelect::create()
{
    auto ret = new HorriblePopupSelect();

    if (ret && ret->initAnchored(300.f, 280.f))
    {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};