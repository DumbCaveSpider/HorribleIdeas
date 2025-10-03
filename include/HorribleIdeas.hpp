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

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// To easily toggle horrible mod options
class HorribleIdeas {
private:
    // Pointer reference to the current mod instance
    static inline Mod* horribleMod = Mod::get();

public:
    /**
     * Returns if the mod is enabled or disabled
     *
     * @param id The ID of the option to check
     *
     * @returns Boolean of the current value
     */
    static bool get(std::string_view id);

    /**
     * Returns the chance value for an option
     *
     * @param id The ID of the option to check
     *
     * @returns Integer of the chance value
     */
    static int getChance(std::string_view id);

    /**
     * Returns if the mod is enabled or disabled
     *
     * @param id The ID of the option to check
     * @param enable True to enable the option, false to disable
     *
     * @returns Boolean of the old value
     */
    static bool set(std::string_view id, bool enable);
};