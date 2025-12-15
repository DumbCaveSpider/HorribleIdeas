#pragma once

#include <Utils.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class ModOption : public CCMenu {
private:
    void saveTogglerState();

protected:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    ModOption();
    virtual ~ModOption();

    void onToggle(CCObject*);
    void onDescription(CCObject*);
    void onExit() override;

    bool init(CCSize const& size, Option const& option);

public:
    static ModOption* create(CCSize const& size, Option const& option);

    Option getOption() const;
    bool isCompatible() const;
};