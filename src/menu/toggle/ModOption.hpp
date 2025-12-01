#pragma once

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class ModOption : public CCMenu {
protected:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    ModOption();
    virtual ~ModOption();

    void onToggle(CCObject*);
    void onDescription(CCObject*);
    void onExit() override;

    bool init(CCSize const& size, Option option);

private:
    void saveTogglerState();

public:
    static ModOption* create(CCSize const& size, Option option);

    Option getOption() const;
    bool isCompatible() const;
};