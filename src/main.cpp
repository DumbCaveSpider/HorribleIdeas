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

        log::debug("Hooked Cocos layer {}", getID());

        if (horribleMod && horribleMod->getSavedValue<bool>("mock", false)) {
            if (auto b = true) { // change to random chance
                auto mockConfigPath = fmt::format("{}\\mock.json", horribleMod->getSaveDir());
                auto mockConfig = file::readJson(std::filesystem::path(mockConfigPath));

                if (mockConfig.isOk()) {
                    auto mockConfigUnwr = mockConfig.unwrapOr(Value());

                    std::srand(static_cast<unsigned int>(std::time(nullptr)));
                    int randInd = std::rand() % mockConfigUnwr.size(); // random index of the matjson

                    auto lvl = mockConfigUnwr.get(static_cast<size_t>(randInd));

                    if (lvl.isOk()) {
                        auto lvlUnwr = lvl.unwrap();

                        auto id = lvlUnwr.getKey().value_or("");
                        auto percent = lvlUnwr.asInt().unwrapOr(99);

                        if (!id.empty()) {
                            std::string path = fmt::format("{}\\{}.png", horribleMod->getSaveDir(), id);

                            // screenshot sprite
                            auto ss = LazySprite::create({ 192.f, 108.f });
                            ss->setID("mock"_spr);
                            ss->setAnchorPoint({ 0.5, 0.5 });
                            ss->setPosition({ -192.f, -108.f });

                            ss->setLoadCallback([this, ss, percent](Result<> res) {
                                if (res.isOk()) {
                                    // Success: scale and position the sprite
                                    log::info("Sprite loaded successfully");

                                    float yA = static_cast<float>(std::rand()) / RAND_MAX; // first y pos %
                                    float yB = static_cast<float>(std::rand()) / RAND_MAX; // last y pos %

                                    auto percLabelText = fmt::format("{}%", std::to_string(percent));

                                    auto percLabel = CCLabelBMFont::create(percLabelText.c_str(), "bigFont.fnt");
                                    percLabel->setPosition({ ss->getScaledContentWidth() / 2.f, ss->getScaledContentHeight() / 2.f });

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
                                };
                                                });

                            ss->loadFromFile(std::filesystem::path(path));
                            addChild(ss);
                        };
                    };
                };
            };
        };

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
                CircleBaseSize::MediumAlt
            );

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(HorribleMenuLayer::onHorribleButton)
            );
            btn->setID("horribleBtn");

            if (auto menu = typeinfo_cast<CCMenu*>(bottomMenu)) {
                menu->addChild(btn);
                menu->updateLayout(true);
            };
        };

        // TESTING

        // DELETE LATER

        auto ss = CCSprite::createWithSpriteFrameName("GJLargeLock_001.png");
        ss->setPosition({ -192.f, 108.f });
        ss->setZOrder(501);

        addChild(ss);

        float yA = static_cast<float>(std::rand()) / RAND_MAX; // first y pos %
        float yB = static_cast<float>(std::rand()) / RAND_MAX; // last y pos %

        ss->setPositionY(getScaledContentHeight() * yA);
        ss->setRotation(360.f * (yA * yB));

        auto move = CCEaseIn::create(CCMoveTo::create(7.5f, { getScaledContentWidth() + 192.f, getScaledContentHeight() * yB }), 1.f);
        auto rotate = CCEaseOut::create(CCRotateBy::create(12.5f, 45.f), 1.f);

        auto action = CCSpawn::createWithTwoActions(move, rotate);
        ss->runAction(action);

        log::info("Animated sprite successfully");

        // DELETE LATER

        // TESTING

        return true;
    };

    void onHorribleButton(CCObject*) {
        if (auto popup = HorribleMenuPopup::create()) popup->show();
    };
};