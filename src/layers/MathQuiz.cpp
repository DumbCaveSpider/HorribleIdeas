#include "MathQuiz.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool MathQuiz::init() {
    if (!CCLayer::init()) return false;

    setID("math-quiz"_spr);
    setKeypadEnabled(true);

    // blue gradient background layer
    auto background = createLayerBG();
    background->setID("background");

    addChild(background);

    // add menu area
    auto menu = CCMenu::create();
    menu->setID("menu");
    menu->setZOrder(10);

    // get window size
    auto winSize = CCDirector::get()->getWinSize();

    // title
    auto title = CCLabelBMFont::create("Math Quiz!", "goldFont.fnt");
    title->setID("title-text");
    title->setPosition(winSize.width / 2.f, winSize.height - 20.f);

    addChild(title);

    // title
    auto subtitle = CCLabelBMFont::create("It's time to solve a math problem!", "bigFont.fnt");
    subtitle->setID("subtitle-text");
    subtitle->setPosition(winSize.width / 2.f, winSize.height - 40.f);
    subtitle->setScale(0.35);

    addChild(subtitle);

    // info button
    auto infoBtnSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");

    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoBtnSprite,
        this,
        menu_selector(MathQuiz::infoPopup)
    );
    infoBtn->setID("info-button");
    infoBtn->setPosition((menu->getScaledContentWidth() / 2) - 22.5f, (menu->getScaledContentHeight() / 2) - 22.5f);

    menu->addChild(infoBtn);

    // add the menu to the layer
    addChild(menu);

    // create top menu (team website)
    auto topMenu = CCMenu::create();
    topMenu->setID("top-layer");
    topMenu->ignoreAnchorPointForPosition(false);
    topMenu->setAnchorPoint(CCPoint(0.5, 0.5));
    topMenu->setPosition(winSize.width / 2, winSize.height / 2 - 15.f);
    topMenu->setScaledContentSize(CCSize(winSize.width - 25.f, winSize.height - 70.f));

    addChild(topMenu);

    // create the layer
    auto topMenuLayer = CCLayer::create();
    topMenuLayer->setID("menu-layer");
    topMenuLayer->setScaledContentSize(topMenu->getScaledContentSize());

    // sprite
    CCScale9Sprite* topScaleSprite = CCScale9Sprite::create("square02b_001.png");
    topScaleSprite->ignoreAnchorPointForPosition(true);
    topScaleSprite->setScaledContentSize(topMenuLayer->getScaledContentSize());
    topScaleSprite->setColor(ccColor3B(6, 2, 32));
    topScaleSprite->setOpacity(155);

    topMenuLayer->addChild(topScaleSprite);
    topMenu->addChild(topMenuLayer);

    // LOGIC THAT SETS UP A RANDOM MATH PROBLEM HERE

    return true;
};

void MathQuiz::infoPopup(CCObject*) {
    FLAlertLayer::create(
        "Richard's Math Quiz",
        "<cr>Developer.</c> This is an <cy>example layer</c> for the ease of <cp>Horrible Ideas</c> developement. This interface is here to serve as a template if more layers are to be created.",
        "OK")
        ->show();
};

// go back to the previous layer
void MathQuiz::onGoBack(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, MenuLayer::scene(false)));
};

// same thing as onGoBack but when escape key is pressed
void MathQuiz::keyBackClicked() {
    onGoBack(nullptr);
};

// create a new instance of the Layer
MathQuiz* MathQuiz::create() {
    auto ret = new MathQuiz();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};

// create a new scene with the Layer
MathQuiz* MathQuiz::scene() {
    auto layer = MathQuiz::create();
    switchToScene(layer);
    return layer;
};