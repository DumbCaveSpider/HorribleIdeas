#include "../Rand.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/binding/GameToolbox.hpp>

using namespace geode::prelude;
using namespace horrible;

int Rand::fast() {
    return static_cast<int>(GameToolbox::fast_rand()) % 100;
};

int Rand::tiny() {
    return static_cast<int>(GameToolbox::fast_rand()) % 5000;
};

int Rand::max() {
    return static_cast<int>(GameToolbox::fast_rand());
};