#include "../Randng.hpp"

#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/binding/GameToolbox.hpp>

using namespace geode::prelude;
using namespace horrible;

int randng::get(int max, int min) {
    return rng::generate<int>(min, max + 1);
};

int randng::tiny() {
    return get(5000, 0);
};

int randng::fast() {
    return get(100, 0);
};