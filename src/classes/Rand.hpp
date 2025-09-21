#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    class Rand {
    public:
        // Get a number between 0 and 100
        static int fast();
    };
};