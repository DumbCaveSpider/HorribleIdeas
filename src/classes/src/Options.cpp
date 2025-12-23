#include "../Options.hpp"

#include <Utils.hpp>

using namespace horrible;

std::vector<Option> const& options::getAll() {
    if (auto om = OptionManager::get()) return om->getOptions();

    static const std::vector<Option> ret;
    return ret;
};

bool options::get(std::string_view id) {
    if (auto om = OptionManager::get()) return om->getOption(id);
    return false;
};

int options::getChance(std::string_view id) {
    return static_cast<int>(horribleMod->getSettingValue<int64_t>(fmt::format("{}-chance", id)));
};

bool options::set(std::string_view id, bool enable) {
    if (auto om = OptionManager::get()) return om->setOption(id, enable);
    return false;
};

std::vector<std::string> const& options::getAllCategories() {
    if (auto om = OptionManager::get()) return om->getCategories();

    static const std::vector<std::string> ret;
    return ret;
};

bool options::doesCategoryExist(std::string_view category) {
    return str::containsAny(category.data(), getAllCategories());
};