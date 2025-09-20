#pragma once

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class HorribleMenuPopup : public Popup<> {
protected:
    bool setup() override;
    void openModSettings(CCObject* sender);

public:
    static HorribleMenuPopup* create();

    static std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>> getAllOptions();
};