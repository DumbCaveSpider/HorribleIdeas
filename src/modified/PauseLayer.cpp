#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
    };
};