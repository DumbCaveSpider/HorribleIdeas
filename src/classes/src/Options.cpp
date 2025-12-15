#include "../Options.hpp"

#include <Utils.hpp>

using namespace horrible;

std::vector<Option> options::getAll() {
    if (auto om = OptionManager::get()) return om->getOptions();
    return {};
};

bool options::get(std::string_view id) {
    if (auto om = OptionManager::get()) return om->getOption(id);
    return false;
};

int options::getChance(std::string_view id) {
    auto fullId = fmt::format("{}-chance", id);
    return static_cast<int>(horribleMod->getSettingValue<int64_t>(fullId));
};

bool options::set(std::string const& id, bool enable) {
    if (auto om = OptionManager::get()) return om->setOption(id, enable);
    return false;
};

std::vector<std::string> options::getAllCategories() {
    if (auto om = OptionManager::get()) return om->getCategories();
    return {};
};

bool options::doesCategoryExist(std::string const& category) {
    for (const auto& cat : getAllCategories()) {
        if (cat == category) return true;
    };

    return false;
};