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
        const std::string m_id;
        const bool m_toggled;

    public:
        HorribleOptionEventV2(std::string_view id, bool toggled)
            : m_id(id), m_toggled(toggled) {};

        std::string const& getId() const { return m_id; };
        bool getToggled() const { return m_toggled; };
    };

    class HorribleOptionEventFilterV2 : public EventFilter<HorribleOptionEventV2> {
    private:
        const std::vector<std::string> m_ids;

    public:
        using Callback = ListenerResult(HorribleOptionEventV2*);

        ListenerResult handle(std::function<Callback> fn, HorribleOptionEventV2* event) {
            for (auto const& id : m_ids) {
                if (event->getId() == id) return fn(event);
            };

            return ListenerResult::Propagate;
        };

        HorribleOptionEventFilterV2() = default;
        HorribleOptionEventFilterV2(std::string_view id) : m_ids({ id.data() }) {};
        HorribleOptionEventFilterV2(std::vector<std::string_view> const& ids) : m_ids(ids.begin(), ids.end()) {};
    };

    class OptionManagerV2 {
    public:
        static Result<> registerOption(Option const& option)
            GEODE_EVENT_EXPORT(&OptionManagerV2::registerOption, (option));

        static Result<bool> getOption(std::string_view id)
            GEODE_EVENT_EXPORT(&OptionManagerV2::getOption, (id));

        static Result<bool> setOption(std::string_view id, bool enable)
            GEODE_EVENT_EXPORT(&OptionManagerV2::setOption, (id, enable));
    };
};