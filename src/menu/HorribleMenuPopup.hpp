#pragma once

#include <Horrible.hpp>

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

    HorribleMenuPopup();
    virtual ~HorribleMenuPopup();

    void filterOptionsByTier(const std::vector<Option>& allOptions, SillyTier tier);
    void filterTierCallback(CCObject*);

    void openModSettings(CCObject*);
    void openSeriesPage(CCObject*);
    void openSupporterPopup(CCObject*);

    bool setup() override;

public:
    static HorribleMenuPopup* create();
};