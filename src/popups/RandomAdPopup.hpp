#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RandomAdPopup : public Popup<> {
protected:
    void onPlayBtn(CCObject*);
    bool setup() override;

public:
    static RandomAdPopup* create();
};