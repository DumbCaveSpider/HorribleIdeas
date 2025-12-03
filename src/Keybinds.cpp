#include <Geode/Geode.hpp>
#include <Geode/utils/cocos.hpp>

#include "menu/HorribleMenuPopup.hpp"

using namespace geode::prelude;

#if !defined(GEODE_IS_IOS)

#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace keybinds;

$execute {
      if (auto bm = BindManager::get()) {
            bm->registerBindable(
                {"popup"_spr,
                 "Show Menu",
                 "Open the Horrible Ideas mod menu.",
                 {Keybind::create(KEY_Backslash, Modifier::None)},
                 "Horrible Ideas"});


            static Ref<HorribleMenuPopup> menuPopup = nullptr;
            new EventListener([=](InvokeBindEvent* event) {
                  if (event->isDown()) {
                        if (menuPopup) {
                              log::warn("Menu popup already open");

                              menuPopup->removeMeAndCleanup();
                              menuPopup = nullptr;
                        } else {
                              menuPopup = HorribleMenuPopup::create();
                              if (menuPopup) menuPopup->show();
                        };
                  };

                  return ListenerResult::Propagate;
            },
                              InvokeBindFilter(nullptr, "popup"_spr));


      } else {
            log::error("Failed to get keybind manager");
      };
};

#endif