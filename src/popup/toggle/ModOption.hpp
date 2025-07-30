#include "../../SillyTier.hpp"

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

using namespace geode::prelude;

class ModOption : public CCMenu
{
protected:
    Mod *m_mod = getMod(); // Horrible Ideas

    std::string m_modID;          // The ID of the horrible mod
    std::string m_modName;        // The name of the horrible mod
    std::string m_modDescription; // The description of the horrible mod
    int m_modTier = 0;            // 0 = none, 1 = low, 2 = medium, 3 = high

    bool m_restartRequired; // If this mod requires a game restart

    CCMenuItemToggler *m_toggler = nullptr; // The toggler for the horrible mod

    void onToggle(CCObject *);
    void onDescription(CCObject *);
    void onExit() override;

    bool init(CCSize const &size, std::string id, std::string name, std::string description, SillyTier silly = SillyTier::None, bool restart = false);

private:
    void saveTogglerState();

public:
    static ModOption *create(CCSize const &size, std::string id, std::string name, std::string description, SillyTier silly = SillyTier::None, bool restart = false);

    std::string getModID();
    std::string getModName();
    std::string getModDescription();
};