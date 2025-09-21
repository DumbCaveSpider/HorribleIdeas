#pragma once

#include <Geode/Geode.hpp>

#include <classes/LevelManager.hpp>
#include <classes/RandomSeeder.hpp>
#include <classes/SillyTier.hpp>

#include <classes/ui/MathQuiz.hpp>
#include <classes/ui/RandomAdPopup.hpp>

using namespace geode::prelude;

// Namespace for utility methods for Horrible Ideas
namespace horrible {
    // Pointer reference to the current mod instance
    inline Mod* horribleMod = Mod::get();
};