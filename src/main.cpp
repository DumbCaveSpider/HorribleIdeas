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

class $modify(HorribleCCLayer, CCLayer) {
    bool init() {
        if (!CCLayer::init()) return false;

        log::debug("Hooked Cocos layer {}", this->getID());

        queueInMainThread([this]() {
            auto del = CCDelayTime::create(0.25f); // use another cc class 

            if (horribleMod && horribleMod->getSavedValue<bool>("upside-down", false)) {
                if ((rand() % 50) == 0) setRotation(-180.f);
            };
                          });

        return true;
    };
};

// modify CCMenuItem so it plays the sound whenever a button is clicked regardless of the layer
class $modify(HorribleCCMenuItem, CCMenuItem) {
    void activate() override {
        if (horribleMod && horribleMod->getSavedValue<bool>("achieve", true)) {
            if (auto fmod = FMODAudioEngine::sharedEngine()) {
                // @geode-ignore(unknown-resource)
                if ((rand() % 75) == 0) fmod->playEffect("achievement_01.ogg"); // 75% chance of playing
            };
        };

        CCMenuItem::activate();
    };
};

class $modify(HorribleMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (auto bottomMenu = this->getChildByID("bottom-menu")) {
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

            if (auto menu = typeinfo_cast<CCMenu*>(bottomMenu)) {
                menu->addChild(btn);
                menu->updateLayout(true);
            };
        };

        // Show a LazySprite for the first PNG found in the save directory
        if (horribleMod && horribleMod->getSavedValue<bool>("mock", false)) {
            log::debug("mock feature enabled in MainMenu layer");

            namespace fs = std::filesystem;

            if ((rand() % 75) == 0) {
                auto mockConfigPath = fmt::format("{}\\mock.json", horribleMod->getSaveDir());
                auto mockConfig = file::readJson(fs::path(mockConfigPath));

                log::debug("Reading path {}...", mockConfigPath);

                if (mockConfig.isOk()) {
                    log::debug("Read mocking config file");

                    auto mockConfigUnwr = mockConfig.unwrapOr(Value());

                    auto lvlUnwr = mockConfigUnwr.begin();
                    std::advance(lvlUnwr, std::rand() % mockConfigUnwr.size());

                    auto id = lvlUnwr->getKey().value_or("");
                    auto percent = lvlUnwr->asInt().unwrapOr(99);

                    if (!id.empty()) {
                        log::debug("ID {} with percentage {} is valid", id, percent);

                        std::string pngPath = fmt::format("{}\\{}.png", horribleMod->getSaveDir(), id);;

                        log::info("Displaying {}", pngPath);

                        auto ss = LazySprite::create({ 192.f, 108.f });
                        ss->setID("mock"_spr);
                        ss->setScale(0.25);
                        ss->setAnchorPoint({ 0.5, 0.5 });
                        ss->setZOrder(1000);
                        ss->setPosition({ -192.f, -108.f });

                        ss->setLoadCallback([this, ss, percent](Result<> res) {
                            if (res.isOk()) {
                                log::info("Sprite loaded successfully from save dir PNG");

                                auto percLabelText = fmt::format("{}%", percent);

                                auto percLabel = CCLabelBMFont::create(percLabelText.c_str(), "bigFont.fnt");
                                percLabel->setID("percentage");
                                percLabel->setPosition({ ss->getScaledContentWidth() / 2.f, ss->getScaledContentHeight() / 2.f });
                                percLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
                                percLabel->ignoreAnchorPointForPosition(false);
                                percLabel->setAnchorPoint({ 0, 0 });
                                percLabel->setScale(2.5);

                                ss->addChild(percLabel);

                                float yA = static_cast<float>(std::rand()) / RAND_MAX;
                                float yB = static_cast<float>(std::rand()) / RAND_MAX;

                                ss->setPositionY(getScaledContentHeight() * yA);
                                ss->setRotation(360.f * (yA * yB));

                                auto move = CCEaseIn::create(CCMoveTo::create(10.f, { getScaledContentWidth() + 192.f, getScaledContentHeight() * yB }), 1.f);
                                auto rotate = CCEaseOut::create(CCRotateBy::create(12.5f, 45.f), 1.f);

                                auto action = CCSpawn::createWithTwoActions(move, rotate);
                                ss->runAction(action);

                                log::info("Animated sprite successfully");
                            } else {
                                log::error("Sprite failed to load: {}", res.unwrapErr());
                                ss->removeMeAndCleanup();
                            };
                                            });

                        ss->loadFromFile(fs::path(pngPath));
                        addChild(ss);
                    } else {
                        log::error("ID is invalid");
                    };
                } else {
                    log::error("Mocking data file not found");
                };
            };
        };

        return true;
    };

    void onHorribleButton(CCObject*) {
        if (auto popup = HorribleMenuPopup::create()) popup->show();
    };
};