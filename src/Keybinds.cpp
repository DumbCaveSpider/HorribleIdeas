#include "popup/HorribleMenuPopup.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_IOS

#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace keybinds;

$execute
{
    if (auto bm = BindManager::get())
    {
        bm->registerBindable({"popup"_spr,
                              "Show Menu",
                              "Show the Horrible Ideas mod menu.",
                              {Keybind::create(KEY_Tab, Modifier::None)},
                              "Horrible Ideas"});

        // optional api version
        static HorribleMenuPopup *s_popup = nullptr;
        new EventListener([=](InvokeBindEvent *event)
                          {
            if (event->isDown()) {
                if (s_popup && s_popup->getParent()) {
                    s_popup->removeFromParent();
                    s_popup = nullptr;
                } else {
                    s_popup = HorribleMenuPopup::create();
                    if (s_popup) s_popup->show();
                }
            }
            return ListenerResult::Propagate; }, InvokeBindFilter(nullptr, "popup"_spr));
    }
    else
    {
        log::error("Failed to get keybind manager");
    };
};

#endif