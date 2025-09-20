#pragma once

namespace horrible {
    // How silly a mod option is
    enum class SillyTier {
        None = 0, // Null
        Low = 1, // Not so silly
        Medium = 2, // Somewhat silly
        High = 3 // Very silly
    };

    class Silly {
    public:
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