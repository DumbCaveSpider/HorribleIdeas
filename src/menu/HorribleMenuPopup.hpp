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

<<<<<<< HEAD
    Ref<ScrollLayer> m_optionList = nullptr;
    Ref<TextInput> m_searchInput = nullptr;
=======
    HorribleMenuPopup();
    virtual ~HorribleMenuPopup();
>>>>>>> fa458e54e8808e051d70f9dcf6c4a95d78aecdbd

    void filterOptionsByTier(const std::vector<Option>& allOptions, SillyTier tier);
    void filterTierCallback(CCObject*);

    void openModSettings(CCObject*);
    void openSeriesPage(CCObject*);
    void openSupporterPopup(CCObject*);

    bool setup() override;

public:
    static HorribleMenuPopup* create();
};