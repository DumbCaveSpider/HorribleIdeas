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

    auto optionsContent = OptionsScrollLayer->m_contentLayer;

    auto modOptions = ModOption::getAllOptions();

    // Sort mod options alphabetically by name
    std::sort(modOptions.begin(), modOptions.end(), [](const auto &a, const auto &b)
              { return std::get<1>(a) < std::get<1>(b); });

    // Set the width for all ModOption backgrounds
    ModOption::setOptionWidth(optionsContent->getContentSize().width - 10.f);

    // Use ColumnLayout for dynamic positioning
    auto columnLayout = ColumnLayout::create();
    columnLayout->setGap(10.f);
    columnLayout->setAxisReverse(true); // Top to bottom
    columnLayout->setAxisAlignment(AxisAlignment::End);

    optionsContent->setLayout(columnLayout);

    for (const auto &option : modOptions)
    {
        const auto &[id, name, desc, restart] = option;
        if (auto modOption = ModOption::create(id, name, desc, restart))
        {
            modOption->setAnchorPoint({0, 1});
            modOption->setPosition({0, 0});
            optionsContent->addChild(modOption);
        }
    }

    optionsContent->updateLayout();
    optionsContent->setContentSize(OptionsScrollLayer->getContentSize());

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