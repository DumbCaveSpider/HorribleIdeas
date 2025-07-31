#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RandomAdPopup : public Popup<> {
protected:
    bool setup() override;

public:
    static RandomAdPopup* create();
};