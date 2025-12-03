#include "../Randng.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/binding/GameToolbox.hpp>

using namespace geode::prelude;
using namespace horrible;

int randng::get(int max, int min) {
    return rng::generate<int>(min, max);
};

int randng::tiny() {
    return get(5000, 0);
};

int randng::fast() {
    return get(100, 0);
};