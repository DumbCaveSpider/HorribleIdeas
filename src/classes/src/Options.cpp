#include "../Options.hpp"

#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

using namespace horrible;
using namespace horribleideas;

namespace horrible {
    std::vector<Option> options::getAll() {
        if (auto optionManager = OptionManager::get()) return optionManager->getOptions();
        return {};
    };

    bool options::get(std::string_view id) {
        if (auto optionManager = OptionManager::get()) return optionManager->getOption(id);
        return false;
    };

    int options::getChance(std::string_view id) {
        auto fullId = fmt::format("{}-chance", id);
        return static_cast<int>(horribleMod->getSettingValue<int64_t>(fullId));
    };

    bool options::set(std::string_view id, bool enable) {
        if (auto optionManager = OptionManager::get()) return optionManager->setOption(id, enable);
        return false;
    };
};