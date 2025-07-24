#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
public:
    bool init() override {
        if (!MenuLayer::init())
            return false;

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (bottomMenu != nullptr) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_moonsIcon_001.png",
                1.0f,
                CircleBaseColor::Green,
                CircleBaseSize::Medium);

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(MyMenuLayer::onMyButton)
            );
            btn->setID("horribleBtn");
            auto menu = typeinfo_cast<CCMenu*>(bottomMenu);
            if (menu != nullptr) {
                menu->addChild(btn);
                menu->updateLayout(true);
            }
        }
        return true;
    }

    void onMyButton(CCObject*) {
        FLAlertLayer::create("Button Pressed", "You pressed the custom button!", "OK")->show();
    }
};