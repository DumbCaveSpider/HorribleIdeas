#include <Geode/Geode.hpp>
#include "HorriblePopupSelect.hpp"

bool HorriblePopupSelect::setup() {
    setTitle("Horrible Options");

    log::debug("do stuff!");

    return true;
};

HorriblePopupSelect* HorriblePopupSelect::create() {
    auto ret = new HorriblePopupSelect();

    if (ret && ret->initAnchored(300.f, 280.f)) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};