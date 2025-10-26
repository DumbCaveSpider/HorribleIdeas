#pragma once

#ifdef GEODE_IS_WINDOWS
#ifdef AWCW_HORRIBLE_API_EXPORTING
#define AWCW_HORRIBLE_API_DLL __declspec(dllexport)
#else
#define AWCW_HORRIBLE_API_DLL __declspec(dllimport)
#endif
#else
#ifdef AWCW_HORRIBLE_API_EXPORTING
#define AWCW_HORRIBLE_API_DLL __attribute__((visibility("default")))
#else
#define AWCW_HORRIBLE_API_DLL
#endif
#endif

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    struct AWCW_HORRIBLE_API_DLL Option; // forward-declare existing type
};

// Namespace with all horrible option functions
namespace horribleideas {
    /**
     * Returns if the mod is enabled or disabled
     *
     * @param id The ID of the option to check
     *
     * @returns Boolean of the current value
     */
    AWCW_HORRIBLE_API_DLL bool get(std::string_view id);

    /**
     * Returns the chance value for an option
     *
     * @param id The ID of the option to check
     *
     * @returns Integer of the chance value
     */
    AWCW_HORRIBLE_API_DLL int getChance(std::string_view id);

    /**
     * Returns if the mod is enabled or disabled
     *
     * @param id The ID of the option to check
     * @param enable True to enable the option, false to disable
     *
     * @returns Boolean of the old value
     */
    AWCW_HORRIBLE_API_DLL bool set(std::string_view id, bool enable);

    // Register an option from another mod. Safe to call during that mod's load.
    AWCW_HORRIBLE_API_DLL void registerOption(const horrible::Option& option);

    // Returns a snapshot of all registered options (including core ones).
    AWCW_HORRIBLE_API_DLL std::vector<horrible::Option> getRegisteredOptions();
};