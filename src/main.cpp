#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "popup/HorriblePopupSelect.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (auto bottomMenu = this->getChildByID("bottom-menu")) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_moonsIcon_001.png",
                1.0f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(MyMenuLayer::onHorribleButton)
            );
            btn->setID("horribleBtn");

            if (auto menu = typeinfo_cast<CCMenu*>(bottomMenu)) {
                menu->addChild(btn);
                menu->updateLayout(true);
            };
        };

        /*
        weird pog rub emoji
        */

        auto pogRubEmoji = LazySprite::create({ 20.f, 20.f }, false);
        pogRubEmoji->setID("pogrubEmoji"_spr);

        pogRubEmoji->setLoadCallback([this, pogRubEmoji](Result<> res) {
            if (res.isOk()) {
                // Success: scale and position the sprite
                log::info("Sprite loaded successfully");
            } else {
                // Failure: set fallback image
                log::error("Sprite failed to load, setting fallback: {}", res.unwrapErr());
                pogRubEmoji->initWithSpriteFrameName("btn_chatHistory_001.png");
            };

            pogRubEmoji->setPosition({ getScaledContentSize().width - 12.5f, 12.5f });
            pogRubEmoji->setAnchorPoint({ 1, 0 });
            pogRubEmoji->setScale(0.875f);
                                     });

        pogRubEmoji->loadFromUrl("https://cdn.discordapp.com/emojis/831564390439321651.webp?size=1024&animated=true", LazySprite::Format::kFmtWebp, false);
        SceneManager::get()->keepAcrossScenes(pogRubEmoji);

        return true;
    };

    void onHorribleButton(CCObject*) {
        if (auto popup = HorriblePopupSelect::create()) popup->show();
    };
};