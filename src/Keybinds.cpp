#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

#if !defined(GEODE_IS_IOS)
#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace keybinds;

$execute{
    if (auto bm = BindManager::get()) {
        bm->registerBindable({
            "popup"_spr,
            "Show Menu",
            "Open the Horrible Ideas mod menu.",
            {
                Keybind::create(KEY_Backslash, Modifier::None)
            },
            "Horrible Ideas"
                               });

            new EventListener([=](InvokeBindEvent* event) {
                if (event->isDown()) menu::open();
                return ListenerResult::Propagate;
                              },
                              InvokeBindFilter(nullptr, "popup"_spr));
    } else {
        log::error("Failed to get keybind manager");
    };
};
#endif