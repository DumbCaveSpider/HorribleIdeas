#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ModOption : public CCMenu {
protected:
    std::string m_modID; // The ID of the horrible mod 
    std::string m_modName; // The name of the horrible mod 
    std::string m_modDescription; // The description of the horrible mod 

    bool m_restartRequired; // If this mod requires a game restart

    CCMenuItemToggler* m_toggler = nullptr; // The toggler for the horrible mod

    void onToggle(CCObject*);
    void onDescription(CCObject*);

    bool init(std::string id, std::string name, std::string description, bool restart = false);

public:
    static ModOption* create(std::string id, std::string name, std::string description, bool restart = false);
};