#include "../Randng.hpp"

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/utils/random.hpp>

#include <Geode/binding/GameToolbox.hpp>

using namespace geode::prelude;
using namespace horrible;

int randng::get(int max) {
    auto maxStr = utils::numToString(max);
    auto rn = utils::random::generateString(maxStr.size(), "0123456789");
    auto num = utils::numFromString<int>(std::string_view(rn), 10);

    if (num.isOk()) {
        auto n = num.unwrapOr(max);
        if (n >= max) n = max;
        return n;
    } else {
        return max;
    };
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