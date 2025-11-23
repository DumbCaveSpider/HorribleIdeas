#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(FriendsPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("friends");
    };

    void setupHasCompleted() {
        if (m_fields->enabled) log::info("random difficulty faces appear across the screen randomly");

        PlayLayer::setupHasCompleted();
    };
};