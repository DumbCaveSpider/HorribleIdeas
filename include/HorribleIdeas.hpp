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

// Namespace with all horrible option functions
namespace horribleideas {
    // Default horrible categories
    class Category {
    public:
        static inline const std::string playerlife = "Player Life";
        static inline const std::string jumpscares = "Jumpscares";
        static inline const std::string randoms = "Randoms";
        static inline const std::string chances = "Chances";
        static inline const std::string obstructive = "Obstructive";
        static inline const std::string misc = "Misc";
    };

    // How silly a mod option is
    enum class AWCW_HORRIBLE_API_DLL SillyTier {
        None = 0,   // Null
        Low = 1,    // Not so silly
        Medium = 2, // Somewhat silly
        High = 3    // Very silly
    };

    // A horrible mod option
    struct AWCW_HORRIBLE_API_DLL Option {
        std::string id; // Unique ID of the option
        std::string name; // Name of the option
        std::string description; // Description of the option
        std::string category; // Name of the category this option should be under
        SillyTier silly; // How silly the option is
        bool restart = false; // If the option requires a restart to take effect
        std::vector<PlatformID> platforms = { PlatformID::Desktop, PlatformID::Mobile }; // Platforms that the option supports

        // Full constructor second
        Option(
            std::string id,
            std::string name,
            std::string description,
            std::string category,
            SillyTier silly,
            bool restart = false,
            std::vector<PlatformID> platforms = { PlatformID::Desktop, PlatformID::Mobile }) : id(std::move(id)),
            name(std::move(name)),
            description(std::move(description)),
            category(std::move(category)),
            silly(silly),
            restart(restart),
            platforms(std::move(platforms)) {};
    };

    // Event for option toggles
    class HorribleOptionEvent : public Event {
    protected:
        std::string id;
        bool isToggled;

    public:
        HorribleOptionEvent(std::string id, bool isToggled);

        AWCW_HORRIBLE_API_DLL std::string getId() const; // Get the unique ID of the option
        AWCW_HORRIBLE_API_DLL bool getIsToggled() const; // Get the toggle boolean of the option
    };

    // Filter for option toggle event
    class AWCW_HORRIBLE_API_DLL HorribleOptionEventFilter : public EventFilter<HorribleOptionEvent> {
    public:
        using Callback = ListenerResult(HorribleOptionEvent*);

        /**
         * Event handler
         *
         * @param fn Callback function containing a pointer to the event that fired
         * @param event Pointer to the event that fired
         */
        ListenerResult handle(std::function<Callback> fn, HorribleOptionEvent* event) {
            return fn(event);
        };
    };

    // Mod option manager for Horrible Ideas
    class AWCW_HORRIBLE_API_DLL OptionManager : public cocos2d::CCObject {
    protected:
        std::vector<Option> m_options; // Array of registered options
        mutable std::mutex m_mutex; // Mutex for thread safety

        std::vector<std::string> m_categories; // Array of auto-registered categories

        OptionManager() = default; // Constructor

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