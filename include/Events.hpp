#pragma once

#include "Horrible.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Container for Horrible Ideas API
namespace horrible {
    // Event for option toggles
    class HorribleOptionEvent : public Event {
    private:
        std::string m_id; // Unique ID of the option
        bool m_toggled; // Toggle boolean of the option

    public:
        HorribleOptionEvent(std::string const& id, bool toggled); // Constructor

        AWCW_HORRIBLE_API_DLL std::string const& getId() const; // Get the unique ID of the option
        AWCW_HORRIBLE_API_DLL bool getToggled() const; // Get the toggle boolean of the option
    };

    // Filter for option toggle event
    class AWCW_HORRIBLE_API_DLL HorribleOptionEventFilter : public EventFilter<HorribleOptionEvent> {
    private:
        std::vector<std::string_view> m_ids = {}; // Unique ID of the options to listen to

    public:
        using Callback = ListenerResult(HorribleOptionEvent*);

        /**
         * Event handler
         *
         * @param fn Callback function containing a pointer to the event that fired
         * @param event Pointer to the event that fired
         */
        ListenerResult handle(std::function<Callback> fn, HorribleOptionEvent* event);

        HorribleOptionEventFilter() = default; // Constructor
        HorribleOptionEventFilter(std::string const& id); // Constructor (listens to one option's toggle)
        HorribleOptionEventFilter(std::vector<std::string_view> const& ids); // Constructor (listens to any specified options' toggles)
    };
};