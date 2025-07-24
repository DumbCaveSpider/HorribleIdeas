#include "HorriblePopupSelect.hpp"

#include "toggle/ModOption.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool HorriblePopupSelect::setup() {
    setID("options"_spr);
    setTitle("Horrible Options");

    // just a test!
    if (auto test = ModOption::create(
        "test-option",
        "Test Option",
        "This is a test option.",
        true
    )) {
        test->setPosition(12.5f, 200.f);
        m_mainLayer->addChild(test);
    };

    return true;
};

HorriblePopupSelect* HorriblePopupSelect::create() {
    auto ret = new HorriblePopupSelect();

    if (ret && ret->initAnchored(300.f, 280.f)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};