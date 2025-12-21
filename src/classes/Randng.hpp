#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    // Random number generator utilities
    namespace randng {
        /**
         * Get a random number between `min` and `max`
         *
         * @param max The maximum number to get
         * @param min The minimum number to get
         */
        int get(int max = 200, int min = 0);

        /**
         * Get any number between 0 and 5000
         * @note Recommended to balance chances when calling every frame
         */
        int tiny();

        // Get any number between 0 and 100
        int fast();
    };
};