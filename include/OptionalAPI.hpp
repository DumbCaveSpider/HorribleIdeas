#pragma once

#include "Horrible.hpp"

#include <Geode/Geode.hpp>

#include <Geode/loader/Dispatch.hpp>

#ifdef MY_MOD_ID
#undef MY_MOD_ID
#endif
#define MY_MOD_ID "arcticwoof.horrible_ideas"

using namespace geode::prelude;

namespace horrible {
    class HorribleOptionEventV2 : public Event {
    private:
        std::string m_id;
        bool m_toggled;

    public:
        HorribleOptionEventV2(std::string const& id, bool toggled)
            : m_id(id), m_toggled(toggled) {};

        std::string const& getId() const { return m_id; };
        bool getToggled() const { return m_toggled; };
    };

    class HorribleOptionEventFilterV2 : public EventFilter<HorribleOptionEventV2> {
    private:
        std::vector<std::string> m_ids;

    public:
        using Callback = ListenerResult(HorribleOptionEventV2*);

        ListenerResult handle(std::function<Callback> fn, HorribleOptionEventV2* event) {
            for (auto const& id : m_ids) {
                if (event->getId() == id) return fn(event);
            };

            return ListenerResult::Propagate;
        };

        HorribleOptionEventFilterV2() = default;
        HorribleOptionEventFilterV2(std::string const& id) : m_ids{ id } {};
        HorribleOptionEventFilterV2(std::vector<std::string> const& ids) : m_ids(ids) {};
    };

    class OptionManagerV2 {
    public:
        static Result<> registerOption(Option const& option)
            GEODE_EVENT_EXPORT(&OptionManagerV2::registerOption, (option));

        static Result<bool> getOption(std::string_view id)
            GEODE_EVENT_EXPORT(&OptionManagerV2::getOption, (id));

        static Result<bool> setOption(std::string const& id, bool enable)
            GEODE_EVENT_EXPORT(&OptionManagerV2::setOption, (id, enable));
    };
};