#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class HorriblePopupSelect : public Popup<>
{
public:
    bool setup();
    static HorriblePopupSelect *create();
};
