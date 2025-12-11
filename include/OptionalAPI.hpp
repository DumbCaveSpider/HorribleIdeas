#pragma once

#include "HorribleIdeas.hpp"

#include <Geode/Geode.hpp>

#include <Geode/loader/Dispatch.hpp>

#ifdef MY_MOD_ID
#undef MY_MOD_ID
#endif
#define MY_MOD_ID "arcticwoof.horrible_ideas"

using namespace geode::prelude;

namespace horribleideas {
    class HorribleOptionEventV2 : public geode::Event {
    private:
        std::string m_id;
        bool m_toggled;

    public:
        HorribleOptionEventV2(std::string id, bool toggled)
            : m_id(std::move(id)), m_toggled(toggled) {};

        std::string getId() const { return m_id; };
        bool getToggled() const { return m_toggled; };
    };

    class HorribleOptionEventFilterV2 : public geode::EventFilter<HorribleOptionEventV2> {
    private:
        std::vector<std::string> m_ids;

    public:
        using Callback = geode::ListenerResult(HorribleOptionEventV2*);

        geode::ListenerResult handle(std::function<Callback> fn, HorribleOptionEventV2* event) {
            if (std::find(m_ids.begin(), m_ids.end(), event->getId()) != m_ids.end()) {
                return fn(event);
            };

            return geode::ListenerResult::Propagate;
        };

        HorribleOptionEventFilterV2() = default;
        HorribleOptionEventFilterV2(const std::string& id) : m_ids{ id } {}
        HorribleOptionEventFilterV2(std::vector<std::string> ids) : m_ids(std::move(ids)) {}
    };

    struct OptionV2 {
        std::string id;
        std::string name;
        std::string description;
        std::string category;
        SillyTier silly;
        bool restart = false;
        std::vector<PlatformID> platforms = { PlatformID::Desktop, PlatformID::Mobile };

        OptionV2() = default;

        OptionV2(
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

        operator Option() const {
            std::vector<PlatformID> plats;
            plats.reserve(platforms.size());
            for (auto const& p : platforms) {
                plats.push_back(static_cast<PlatformID>(p));
            };
            return Option(
                id,
                name,
                description,
                category,
                static_cast<SillyTier>(silly),
                restart,
                plats
            );
        };
    };

    class OptionManagerV2 {
    public:
        static Result<> registerOption(const OptionV2& option)
            GEODE_EVENT_EXPORT(&OptionManagerV2::registerOption, (option));

        static Result<bool> getOption(std::string_view id)
            GEODE_EVENT_EXPORT(&OptionManagerV2::getOption, (id));

        static Result<bool> setOption(const std::string& id, bool enable)
            GEODE_EVENT_EXPORT(&OptionManagerV2::setOption, (id, enable));
    };
};