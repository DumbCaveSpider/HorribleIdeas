#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    class Rand {
    public:
        // Get a number between 0 and 100
        static int fast();

        // Get a number between 0 and 5000, recommended to balance chances when firing every frame
        static int tiny();

        // Get a random number to 32767
        static int max();

        // Get a random number between 0 and max
        static int get(int max = 200);
    };
};