#pragma once

#include "CategoryItem.hpp"

#include <Utils.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

struct FilterBtnInfo {
    SillyTier tier;
    const char* label;
    ccColor3B color;
};

class HorribleMenuPopup : public Popup<> {
protected:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    static HorribleMenuPopup* s_inst;

    EventListener<CategoryEventFilter> m_listener = {
        [this](CategoryEvent* event) {
            return OnCategory(event->getId(), event->isEnabled());
        },
        CategoryEventFilter()
    };

    HorribleMenuPopup();
    virtual ~HorribleMenuPopup();

    ListenerResult OnCategory(std::string const& category, bool enabled = true);

    void filterOptions(std::vector<Option> const& allOptions, SillyTier tier = SillyTier::None, std::string const& category = "");
    void filterTierCallback(CCObject*);

    void resetFilters(CCObject*);

    void openModSettings(CCObject*);
    void openSeriesPage(CCObject*);
    void openSupporterPopup(CCObject*);

    void onClose(CCObject* sender) override;
    void onExit() override;
    void cleanup() override;

    bool setup() override;

public:
    static HorribleMenuPopup* create();

    static HorribleMenuPopup* get();
};