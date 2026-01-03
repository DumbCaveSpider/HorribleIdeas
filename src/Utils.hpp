#pragma once

#include <Horrible.hpp>

#include <fmt/core.h>

#include <Geode/Geode.hpp>

#include <Geode/loader/SettingV3.hpp>

#include <classes/Colors.hpp>
#include <classes/Jumpscares.hpp>
#include <classes/Menu.hpp>
#include <classes/Options.hpp>
#include <classes/Randng.hpp>

#include <classes/ui/MathQuiz.hpp>
#include <classes/ui/RandomAd.hpp>
#include <classes/ui/SpamChallenge.hpp>

using namespace geode::prelude;

// Additional utility methods for Horrible Ideas
namespace horrible {
    // Pointer reference to this Geode mod
    inline Mod* horribleMod = Mod::get();

    /**
     * Convert a chance setting number to a cooldown percentage decimal
     *
     * @param chance The chance setting number
     */
    inline float chanceToDelayPct(int chance) {
        if (chance <= 0) chance = 1;
        if (chance > 100) chance = 100;

        return ((100.f - static_cast<float>(chance)) + 1.f) / 100.f;
    };

    /**
     * Play a sound effect using FMOD
     *
     * @param name Name of the audio file
     */
    inline void playSfx(const char* file) {
        if (auto fmod = FMODAudioEngine::sharedEngine()) fmod->playEffectAsync(file);
    };

    namespace str = utils::string; // Shortcut for geode::utils::string

    // Default horrible categories
    namespace category {
        inline constexpr const char* playerlife = "Player Life";
        inline constexpr const char* jumpscares = "Jumpscares";
        inline constexpr const char* randoms = "Randoms";
        inline constexpr const char* chances = "Chances";
        inline constexpr const char* obstructive = "Obstructive";
        inline constexpr const char* misc = "Misc";
    };
};