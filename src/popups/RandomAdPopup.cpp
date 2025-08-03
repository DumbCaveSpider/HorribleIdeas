#include "RandomAdPopup.hpp"

#include "../utils/LevelManager.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool RandomAdPopup::setup() {
    setID("ad"_spr);
    setTitle("Sponsored");

    auto textLabel = CCLabelBMFont::create("Check out this cool level we found!", "chatFont.fnt");
    textLabel->setID("message");
    textLabel->ignoreAnchorPointForPosition(false);
    textLabel->setPosition({ m_mainLayer->getScaledContentWidth() / 2.f, m_mainLayer->getScaledContentHeight() - 37.5f });
    textLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    textLabel->setAnchorPoint({ 0.5, 0.5 });

    m_mainLayer->addChild(textLabel);

    // featured project thumbnail
    auto projThumb = LazySprite::create({ 192.f, 108.f }, true);
    projThumb->setID("thumbnail");
    projThumb->setAnchorPoint({ 0.5, 0.5 });
    projThumb->setPosition({ m_mainLayer->getContentWidth() / 2.f, 110.f });

    projThumb->setLoadCallback([this, projThumb](Result<> res) {
        if (res.isOk()) {
            log::info("Sprite loaded successfully");

            projThumb->setScale(0.625);
        } else {
            log::error("Sprite failed to load: {}", res.unwrapErr());
            projThumb->removeMeAndCleanup();
        };
                               });

    projThumb->loadFromUrl("https://api.cubicstudios.xyz/avalanche/v1/fetch/random-thumbnail", CCImage::kFmtUnKnown, true);
    if (projThumb) m_mainLayer->addChild(projThumb);

    ButtonSprite* playBtnSprite = ButtonSprite::create("View");

    // takes u to congreg lol
    auto playBtn = CCMenuItemSpriteExtra::create(
        playBtnSprite,
        this,
        menu_selector(RandomAdPopup::onPlayBtn)
    );
    playBtn->setPosition({ m_mainLayer->getScaledContentWidth() / 2.f, 2.5f });
    playBtn->ignoreAnchorPointForPosition(false);
    playBtn->setVisible(true);
    playBtn->setZOrder(3);

    m_buttonMenu->addChild(playBtn);

    return true;
};

void RandomAdPopup::onPlayBtn(CCObject*) {// congregation jumpscare
    LevelManager::DownloadCongregLevel();

    if (auto playLayer = PlayLayer::get()) {
        auto glm = GameLevelManager::get();

        if (auto congregLevel = glm->getSavedLevel(93437568)) {
            playLayer->onExit();

            auto scene = playLayer->scene(congregLevel, false, false);
            CCDirector::get()->replaceScene(scene);

            log::info("Switching to Congregation level (93437568)");
            return;
        } else {
            log::error("Failed to fetch congregation :9");
        };
    } else {
        log::error("Player not in a level");
    };
};

RandomAdPopup* RandomAdPopup::create() {
    auto ret = new RandomAdPopup();

    if (ret && ret->initAnchored(375.f, 250.f)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};