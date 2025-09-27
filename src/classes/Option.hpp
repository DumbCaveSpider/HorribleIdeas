#pragma once

#include "SillyTier.hpp"

namespace horrible {
    struct Option {
        std::string id; // Unique ID of the option
        std::string name; // Name of the option
        std::string description; // Description of the option
        SillyTier silly; // How silly the option is
        bool restart; // If the option requires a restart to take effect

        Option(
            std::string id,
            std::string name,
            std::string description,
            SillyTier silly = SillyTier::None,
            bool restart = false
        ) : id(id), name(name), description(description), silly(silly), restart(restart) {};
    };
};