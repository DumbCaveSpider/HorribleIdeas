#include <HorribleIdeas.hpp>

bool HorribleIdeas::get(std::string_view id) {
    return horribleMod->getSavedValue<bool>(id, false);
};

bool HorribleIdeas::set(std::string_view id, bool enable) {
    return horribleMod->setSavedValue<bool>(id, enable);
};