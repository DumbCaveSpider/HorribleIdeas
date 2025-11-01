#pragma once

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class HorribleMenuPopup : public Popup<> {
protected:
    SillyTier s_selectedTier = SillyTier::None;

    Ref<ScrollLayer> m_optionList = nullptr;

    EventListener<HorribleOptionEventFilter> m_listener = {
        [=](HorribleOptionEvent* event) {
            log::info("Horrible option of ID {} toggled to {}", event->getId(), event->getIsToggled() ? "ON" : "OFF");
            return ListenerResult::Propagate;
        },
        HorribleOptionEventFilter()
    };

    void filterOptionsByTier(const std::vector<Option>& allOptions, SillyTier tier);

    void openModSettings(CCObject* sender);

    void filterTierCallback(CCObject*);
    void openSeriesPage(CCObject*);

    bool setup() override;

public:
    static HorribleMenuPopup* create();

    static std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>> getAllOptions();
};