#include "../Menu.hpp"

#include <Horrible.hpp>
#include <menu/HorribleMenuPopup.hpp>

using namespace horrible;

menu::MenuButtonTarget::MenuButtonTarget() {
      retain();
};

menu::MenuButtonTarget* menu::MenuButtonTarget::get() {
      static auto inst = new MenuButtonTarget();
      return inst;
};

void menu::MenuButtonTarget::openMenuCallback(CCObject*) {
      menu::open();
};

void menu::open() {
      if (auto popup = HorribleMenuPopup::create()) popup->show();
};

void menu::addButton(CCNode* menu, float scale) {
      if (auto m = static_cast<CCMenu*>(menu)) {
            auto btnSprite = CircleButtonSprite::createWithSprite(
                "shocksprite.png"_spr,
                0.9f);
            btnSprite->setScale(scale);

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                MenuButtonTarget::get(),
                menu_selector(MenuButtonTarget::openMenuCallback));
            btn->setID("horrible-options-button"_spr);
            btn->setZOrder(9);

            m->addChild(btn);
            m->updateLayout(true);
      } else {
            log::error("Could not cast node to CCMenu to add mod option menu button");
      };
};