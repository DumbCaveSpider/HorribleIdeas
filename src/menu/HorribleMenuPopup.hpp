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

    ListenerResult OnCategory(const std::string& category, bool enabled = true);

    void filterOptions(const std::vector<Option>& allOptions, SillyTier tier = SillyTier::None, const std::string& category = "");
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