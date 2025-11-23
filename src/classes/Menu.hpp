#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    // Menu-related utilities
    namespace menu {
        // Open the Horrible Ideas mod option menu
        void open();

        /**
         * Add the Horrible Ideas mod option menu button to a menu
         * @param menu The menu to add the button to
         * @param target The target object for the button callback
         * @param callback The callback selector for the button
         * @param scale The scale of the button
         */
        void addButton(CCNode* menu, CCObject* target, SEL_MenuHandler callback, float scale = 1.f);
    };
};