#pragma once

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class ModOption : public CCMenu {
protected:
    bool s_compatible = false; // If this option is compatible with the current platform

    std::string m_modID; // The ID of the option
    std::string m_modName; // The name of the option
    std::string m_modDescription; // The description of the option
    std::string m_modCategory; // The description of the option
    SillyTier m_modTier = SillyTier::None;

    bool m_restart = false; // If this mod requires a game restart on toggle
    std::vector<PlatformID> m_platforms = { PlatformID::Desktop, PlatformID::Mobile }; // The platforms the option supports

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