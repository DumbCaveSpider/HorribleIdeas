#include "../Randng.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/binding/GameToolbox.hpp>

using namespace geode::prelude;
using namespace horrible;

int randng::fast() {
    return static_cast<int>(GameToolbox::fast_rand()) % 100;
};

int randng::tiny() {
    return static_cast<int>(GameToolbox::fast_rand()) % 5000;
};

int randng::max() {
    return static_cast<int>(GameToolbox::fast_rand());
};

int randng::get(int max) {
    return static_cast<int>(GameToolbox::fast_rand()) % max;
};