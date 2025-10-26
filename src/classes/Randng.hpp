#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    // Random number generator utilities
    namespace randng {
        /**
         * Get a number between 0 and 100
         */
        int fast();

        /**
         * Get a number between 0 and 5000, recommended to balance chances when firing every frame
         */
        int tiny();

        /**
         * Get a random number to 32767
         */
        int max();

        /**
         * Get a number between 0 and max
         *
         * @param max The maximum number to get
         */
        int get(int max = 200);
    };
};