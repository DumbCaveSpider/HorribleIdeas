#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class HorriblePopupSelect : public Popup<> {
protected:
    bool setup() override;

public:
    static HorriblePopupSelect* create();
};