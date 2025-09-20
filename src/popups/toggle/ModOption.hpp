#pragma once

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class ModOption : public CCMenu {
protected:
    std::string m_modID; // The ID of the horrible mod
    std::string m_modName; // The name of the horrible mod
    std::string m_modDescription; // The description of the horrible mod
    SillyTier m_modTier = SillyTier::None;

    bool m_restartRequired; // If this mod requires a game restart

    CCMenuItemToggler* m_toggler = nullptr; // The toggler for the horrible mod

    void onToggle(CCObject*);
    void onDescription(CCObject*);
    void onExit() override;

    bool init(CCSize const& size, std::string id, std::string name, std::string description, SillyTier silly = SillyTier::None, bool restart = false);

private:
    void saveTogglerState();

public:
    static ModOption* create(CCSize const& size, std::string id, std::string name, std::string description, SillyTier silly = SillyTier::None, bool restart = false);

    std::string getModID();
    std::string getModName();
    std::string getModDescription();
};