#include <Geode/Geode.hpp>

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_IOS

#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace keybinds;

$execute
{
    if (auto bm = BindManager::get()) {
        bm->registerBindable({
            "popup"_spr,
            "Show Menu", "Show the Horrible Ideas mod menu.",
            { Keybind::create(KEY_Tab, Modifier::None) },
            "Horrible Ideas"
                             });
    } else {
        log::error("Failed to get keybind manager");
    };
};

#endif