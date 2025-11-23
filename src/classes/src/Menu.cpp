#include "../Menu.hpp"

#include <Horrible.hpp>

#include <menu/HorribleMenuPopup.hpp>

using namespace horrible;

void menu::open() {
    if (auto popup = HorribleMenuPopup::create()) popup->show();
};

void menu::addButton(CCNode* menu, CCObject* target, SEL_MenuHandler callback, float scale) {
    auto btnSprite = CircleButtonSprite::createWithSprite(
        "shocksprite.png"_spr,
        0.9f,
        CircleBaseColor::Green,
        CircleBaseSize::MediumAlt
    );
    btnSprite->setScale(scale);

    auto btn = CCMenuItemSpriteExtra::create(
        btnSprite,
        target,
        callback
    );
    btn->setID("horrible-menu-btn"_spr);

    if (auto m = static_cast<CCMenu*>(menu)) {
        m->addChild(btn);
        m->updateLayout(true);
    };
};