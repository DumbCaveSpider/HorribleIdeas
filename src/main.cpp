#include "popup/HorribleMenuPopup.hpp"

#include <fmt/core.h>

#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/modify/CCMenuItem.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;
using namespace geode::utils;
using namespace matjson;

auto horribleMod = getMod();

class $modify(HorribleCCLayer, CCLayer)
{
    bool init()
    {
        if (!CCLayer::init())
            return false;

        log::debug("Hooked Cocos layer {}", this->getID());

        queueInMainThread([this]()
                          {
            auto del = CCDelayTime::create(0.25f); // use another cc class 

            if (horribleMod && horribleMod->getSavedValue<bool>("upside-down", false)) {
                if ((rand() % 50) == 0) setRotation(-180.f);
            }; });

        return true;
    };
};

// modify CCMenuItem so it plays the sound whenever a button is clicked regardless of the layer
class $modify(HorribleCCMenuItem, CCMenuItem)
{
    void activate() override
    {
        if (horribleMod && horribleMod->getSavedValue<bool>("achieve", true))
        {
            if (auto fmod = FMODAudioEngine::sharedEngine())
            {
                // @geode-ignore(unknown-resource)
                if ((rand() % 75) == 0)
                    fmod->playEffect("achievement_01.ogg"); // 75% chance of playing
            };
        };

        CCMenuItem::activate();
    };
};

class $modify(HorribleMenuLayer, MenuLayer)
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
                menu_selector(HorribleMenuLayer::onHorribleButton));
            btn->setID("horribleBtn");

            if (auto menu = typeinfo_cast<CCMenu *>(bottomMenu))
            {
                menu->addChild(btn);
                menu->updateLayout(true);
            };
        }

        // Show a LazySprite for the first PNG found in the save directory
        if (horribleMod && horribleMod->getSavedValue<bool>("mock", false))
        {
            log::debug("mock feature enabled in MainMenu layer");
            namespace fs = std::filesystem;
            auto saveDir = horribleMod->getSaveDir();
            std::string pngPath;
            for (const auto &entry : fs::directory_iterator(saveDir))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".png")
                {
                    pngPath = entry.path().string();
                    break;
                }
            }
            if (!pngPath.empty())
            {
                log::info("Displaying PNG: {}", pngPath);
                auto ss = LazySprite::create({192.f, 108.f});
                ss->setID("mock"_spr);
                ss->setScale(0.5f);
                ss->setAnchorPoint({0.5, 0.5});
                ss->setZOrder(1000);
                ss->setPosition({-192.f, -108.f});
                ss->setLoadCallback([this, ss](Result<> res)
                                    {
                    if (res.isOk()) {
                        log::info("Sprite loaded successfully from save dir PNG");
                        float yA = static_cast<float>(std::rand()) / RAND_MAX;
                        float yB = static_cast<float>(std::rand()) / RAND_MAX;
                        ss->setPositionY(getScaledContentHeight() * yA);
                        ss->setRotation(360.f * (yA * yB));
                        auto move = CCEaseIn::create(CCMoveTo::create(7.5f, { getScaledContentWidth() + 192.f, getScaledContentHeight() * yB }), 1.f);
                        auto rotate = CCEaseOut::create(CCRotateBy::create(12.5f, 45.f), 1.f);
                        auto action = CCSpawn::createWithTwoActions(move, rotate);
                        ss->runAction(action);
                        log::info("Animated sprite successfully");
                    } else {
                        log::error("Sprite failed to load: {}", res.unwrapErr());
                        ss->removeMeAndCleanup();
                    } });
                ss->loadFromFile(fs::path(pngPath));
                addChild(ss);
            }
        }

        return true;
    };

    void onHorribleButton(CCObject *)
    {
        if (auto popup = HorribleMenuPopup::create())
            popup->show();
    };
};