#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "popup/HorriblePopupSelect.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer)
{
public:
    bool init() override
    {
        if (!MenuLayer::init())
            return false;

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (bottomMenu != nullptr)
        {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_moonsIcon_001.png",
                1.0f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt);

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(MyMenuLayer::onHorribleButton));
            btn->setID("horribleBtn");
            auto menu = typeinfo_cast<CCMenu *>(bottomMenu);
            if (menu != nullptr)
            {
                menu->addChild(btn);
                menu->updateLayout(true);
            }
        }
        return true;
    }

    void onHorribleButton(CCObject *)
    {
        auto popup = HorriblePopupSelect::create();
        popup->show();
    }
};