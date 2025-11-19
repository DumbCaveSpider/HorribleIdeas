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

#include "Events.hpp"
#include "Option.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Container for Horrible Ideas API
namespace horribleideas {
    // Mod option manager for Horrible Ideas
    class AWCW_HORRIBLE_API_DLL OptionManager : public CCObject {
    protected:
        std::vector<Option> m_options; // Array of registered options
        std::vector<std::string> m_categories; // Array of auto-registered categories

        OptionManager(); // Constructor

        /**
         * Register a category if not already registered
         *
         * @param category Name of the category
         */
        void registerCategory(std::string_view category);

    public:
        // Get option manager singleton
        static OptionManager* get();

        /**
         * Register a new option
         *
         * @param option Constructed option object
         */
        void registerOption(const Option& option);

        /**
         * Returns the array of all registered options
         *
         * @returns An array of every registered option, main and external
         */
        std::vector<Option> getOptions() const;

        /**
         * Returns the toggle state of an option
         *
         * @param id The ID of the option to check
         *
         * @returns Boolean of the current value
         */
        bool getOption(std::string_view id) const;

        /**
         * Set the toggle state of an option
         *
         * @param id The ID of the option to toggle
         * @param enable Boolean to toggle to
         *
         * @returns Boolean of the old value
         */
        bool setOption(std::string_view id, bool enable) const;

        /**
         * Returns the array of all registered categories
         *
         * @returns An array of every category name
         */
        std::vector<std::string> getCategories() const;
    };
};