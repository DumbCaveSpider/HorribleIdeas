#pragma once

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class ModOption : public CCMenu {
protected:
    bool s_compatible = false; // If this option is compatible with the current platform

    // The option
    Option m_option = {
        "unk"_spr,
        "Unknown Option",
        "No description provided.",
        "General",
        SillyTier::None
    };

    Ref<CCMenuItemToggler> m_toggler = nullptr; // The toggler for the option

    void onToggle(CCObject*);
    void onDescription(CCObject*);
    void onExit() override;

    bool init(CCSize const& size, Option option);

private:
    void saveTogglerState();

public:
    static ModOption* create(CCSize const& size, Option option);

    std::string getModID();
    std::string getModName();
    std::string getModDescription();
};