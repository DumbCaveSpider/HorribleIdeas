#include <HorribleIdeas.hpp>

#include <fmt/core.h>

bool HorribleIdeas::get(std::string_view id) {
    return horribleMod->getSavedValue<bool>(id, false);
};

int HorribleIdeas::getChance(std::string_view id) {
    auto fullId = fmt::format("{}-chance", id);
    return static_cast<int>(horribleMod->getSettingValue<int64_t>(id)) || 0;
};

bool HorribleIdeas::set(std::string_view id, bool enable) {
    return horribleMod->setSavedValue<bool>(id, enable);
};