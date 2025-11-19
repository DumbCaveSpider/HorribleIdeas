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
    SillyTier s_selectedTier = SillyTier::None;

    Ref<ScrollLayer> m_optionList = nullptr;

    void filterOptionsByTier(const std::vector<Option>& allOptions, SillyTier tier);
    void filterTierCallback(CCObject*);

    void openModSettings(CCObject*);
    void openSeriesPage(CCObject*);
    void openSupporterPopup(CCObject*);

    bool setup() override;

public:
    static HorribleMenuPopup* create();

    static std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>> getAllOptions();
};