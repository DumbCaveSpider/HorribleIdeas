#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    // Easy color object references -w-
    namespace colors {
        inline constexpr ccColor3B white = { 255, 255, 255 };
        inline constexpr ccColor3B gray = { 150, 150, 150 };
        inline constexpr ccColor3B red = { 255, 50, 50 };
        inline constexpr ccColor3B yellow = { 250, 250, 25 };
        inline constexpr ccColor3B green = { 100, 255, 100 };
        inline constexpr ccColor3B cyan = { 10, 175, 255 };
        inline constexpr ccColor3B black = { 0, 0, 0 };
    };
};