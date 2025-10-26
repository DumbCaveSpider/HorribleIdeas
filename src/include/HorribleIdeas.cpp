#include <HorribleIdeas.hpp>

#include <fmt/core.h>

static inline std::mutex s_horrible_registry_mutex;

bool horribleideas::get(std::string_view id) {
    return Mod::get()->getSavedValue<bool>(std::string(id), false);
};

int horribleideas::getChance(std::string_view id) {
    auto fullId = fmt::format("{}-chance", id);
    return static_cast<int>(Mod::get()->getSettingValue<int64_t>(fullId));
};

bool horribleideas::set(std::string_view id, bool enable) {
    return Mod::get()->setSavedValue<bool>(std::string(id), enable);
};

void horribleideas::registerOption(const horrible::Option& option) {
    std::lock_guard<std::mutex> lk(s_horrible_registry_mutex);
    horrible::modOptions.push_back(option);
};

std::vector<horrible::Option> horribleideas::getRegisteredOptions() {
    std::lock_guard<std::mutex> lk(s_horrible_registry_mutex);
    return horrible::modOptions;
};