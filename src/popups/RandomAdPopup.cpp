#include "RandomAdPopup.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool RandomAdPopup::setup() {
    setID("ad"_spr);
    setTitle("Sponsored");

    auto textLabel = CCLabelBMFont::create("Check out this cool level we found!", "chatFont.fnt");
    textLabel->setID("message");
    textLabel->ignoreAnchorPointForPosition(false);
    textLabel->setPosition({ m_mainLayer->getScaledContentWidth() / 2.f, m_mainLayer->getScaledContentHeight() - 12.5f });
    textLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    textLabel->setAnchorPoint({ 0.5, 0.5 });

    m_mainLayer->addChild(textLabel);

    // featured project thumbnail
    auto projThumb = LazySprite::create({ 192.f, 108.f }, true);
    projThumb->setID("thumbnail");
    projThumb->setPosition({ m_mainLayer->getContentWidth() / 2.f, m_mainLayer->getContentHeight() * 0.375f });

    projThumb->setLoadCallback([this, projThumb](Result<> res) {
        if (res.isOk()) {
            log::info("Sprite loaded successfully");

            projThumb->setScale(0.25);
        } else {
            log::error("Sprite failed to load: {}", res.unwrapErr());
            projThumb->removeMeAndCleanup();
        };
                               });

    projThumb->loadFromUrl("https://api.cubicstudios.xyz/avalanche/v1/fetch/random-thumbnail");
    if (projThumb) m_mainLayer->addChild(projThumb);

    return true;
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