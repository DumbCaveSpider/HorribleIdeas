#pragma once

#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <classes/Jumpscares.hpp>
#include <classes/Options.hpp>
#include <classes/Randng.hpp>

#include <classes/ui/MathQuiz.hpp>
#include <classes/ui/RandomAdPopup.hpp>

using namespace geode::prelude;
using namespace horribleideas;

// Namespace for utility methods for Horrible Ideas
namespace horrible {
    // Pointer reference to the current mod instance
    inline Mod* horribleMod = Mod::get();

    // Utilities for SillyTier
    namespace silly {
        /**
         * Get the int for the tier of silly
         *
         * @param silly Silly tier enum
         *
         * @returns The integer from the silly tier
         */
        static int getInt(SillyTier silly) {
            switch (silly) {
            case SillyTier::None:
                return 0; // No silliness

            case SillyTier::Low:
                return 1;

            case SillyTier::Medium:
                return 2;

            case SillyTier::High:
                return 3;

            default:
                return 0;
            };

            return 0;
        };
    };
};