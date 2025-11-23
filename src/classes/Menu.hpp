#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    // Menu-related utilities
    namespace menu {
        class MenuButtonTarget : public CCObject {
        protected:
            MenuButtonTarget(); // Constructor

        public:
            // Get menu button target singleton
            static MenuButtonTarget* get(); 

            void openMenuCallback(CCObject*); // Callback to open the Horrible Ideas mod option menu
        };

        void open(); // Open the Horrible Ideas mod option menu

        /**
         * Add the Horrible Ideas mod option menu button to a menu
         * @param menu The menu to add the button to
         * @param target The target object for the button callback
         * @param scale The scale of the button
         */
        void addButton(CCNode* menu, float scale = 1.f);
    };
};