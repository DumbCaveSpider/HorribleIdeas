#include "popup/HorribleMenuPopup.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/CCMenuItem.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;

auto horribleMod = getMod();

// modify CCMenuItem so it plays the sound whenever a button is clicked regardless of the layer
class $modify(HorribleMenuItem, CCMenuItem)
{
    void activate() override
    {
        auto mod = getMod();
        if (mod && mod->getSavedValue<bool>("achieve", true))
        {
            if (auto fmod = FMODAudioEngine::sharedEngine())
            {
                // @geode-ignore(unknown-resource)
                fmod->playEffect("achievement_01.ogg");
            }
        }
        CCMenuItem::activate();
    }
};

class $modify(MyMenuLayer, MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
            return false;

        if (auto bottomMenu = this->getChildByID("bottom-menu"))
        {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_moonsIcon_001.png",
                0.875f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt);

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(MyMenuLayer::onHorribleButton));
            btn->setID("horribleBtn");

            if (auto menu = typeinfo_cast<CCMenu *>(bottomMenu))
            {
                menu->addChild(btn);
                menu->updateLayout(true);
            };
        };

        return true;
    };

    void onHorribleButton(CCObject *)
    {
        if (auto popup = HorribleMenuPopup::create())
            popup->show();
    };
};