#pragma once

#include "../SillyTier.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class HorriblePopupSelect : public Popup<> {
protected:
    bool setup() override;

public:
    static HorriblePopupSelect* create();

    static std::vector<std::tuple<std::string, std::string, std::string, SillyTier, bool>> getAllOptions();
};