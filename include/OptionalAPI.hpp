#pragma once

#include "HorribleIdeas.hpp"

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
        HorribleOptionEventV2(std::string id, bool toggled)
            : m_id(std::move(id)), m_toggled(toggled) {};

        std::string getId() const { return m_id; };
        bool getToggled() const { return m_toggled; };
    };

    class HorribleOptionEventFilterV2 : public EventFilter<HorribleOptionEventV2> {
    private:
        std::vector<std::string> m_ids;

    public:
        using Callback = ListenerResult(HorribleOptionEventV2*);

        ListenerResult handle(std::function<Callback> fn, HorribleOptionEventV2* event) {
            if (std::find(m_ids.begin(), m_ids.end(), event->getId()) != m_ids.end()) {
                return fn(event);
            };

            return ListenerResult::Propagate;
        };

        HorribleOptionEventFilterV2() = default;
        HorribleOptionEventFilterV2(const std::string& id) : m_ids{ id } {}
        HorribleOptionEventFilterV2(std::vector<std::string> ids) : m_ids(std::move(ids)) {}
    };

    class OptionManagerV2 {
    public:
        static Result<> registerOption(const Option& option)
            GEODE_EVENT_EXPORT(&OptionManagerV2::registerOption, (option));

        static Result<bool> getOption(std::string_view id)
            GEODE_EVENT_EXPORT(&OptionManagerV2::getOption, (id));

        static Result<bool> setOption(const std::string& id, bool enable)
            GEODE_EVENT_EXPORT(&OptionManagerV2::setOption, (id, enable));
    };
};