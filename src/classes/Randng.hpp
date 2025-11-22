#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    // Random number generator utilities
    namespace randng {
        /**
         * Get any number between 0 and max
         *
         * @param max The maximum number to get
         */
        int get(int max = 200);

        /**
         * Get any number between 0 and 5000
         * @note Recommended to balance chances when firing every frame
         */
        int tiny();

        // Get any number between 0 and 100
        int fast();

        // Get a random number up to 32767
        int max();
    };
};