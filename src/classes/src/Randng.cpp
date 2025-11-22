#include "../Randng.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/binding/GameToolbox.hpp>

using namespace geode::prelude;
using namespace horrible;

int randng::get(int max) {
    return static_cast<int>(GameToolbox::fast_rand()) % max;
};

int randng::tiny() {
    return get(5000);
};

int randng::fast() {
    return get(100);
};

int randng::max() {
    return static_cast<int>(GameToolbox::fast_rand());
};