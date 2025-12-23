#pragma once

#include "Horrible.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Container for Horrible Ideas API
namespace horrible {
    // How silly a mod option is
    enum class SillyTier : unsigned int {
        None = 0, // Null
        Low = 1, // Not so silly
        Medium = 2, // Somewhat silly
        High = 3 // Very silly
    };

    // A horrible mod option
    struct Option {
        std::string id; // Unique ID of the option
        std::string name; // Name of the option
        std::string description; // Description of the option
        std::string category; // Name of the category this option should be under
        SillyTier silly = SillyTier::Low; // How silly the option is
        bool restart = false; // If the option requires a restart to take effect
        std::vector<PlatformID> platforms = { PlatformID::Desktop, PlatformID::Mobile }; // Platforms that the option supports

        Option() = default; // Default constructor

        // Constructor
        Option(
            std::string_view id,
            std::string_view name,
            std::string_view description,
            std::string_view category,
            SillyTier silly = SillyTier::Low,
            bool restart = false,
            std::vector<PlatformID> const& platforms = { PlatformID::Desktop, PlatformID::Mobile }
        ) : id(id),
            name(name),
            description(description),
            category(category),
            silly(silly),
            restart(restart),
            platforms(platforms) {};
    };
};