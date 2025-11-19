#include <HorribleIdeas.hpp>

#include <fmt/core.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horribleideas;

HorribleOptionEvent::HorribleOptionEvent(std::string id, bool isToggled)
    : id(std::move(id)), isToggled(isToggled) {};

std::string HorribleOptionEvent::getId() const {
    return id;
};

bool HorribleOptionEvent::getIsToggled() const {
    return isToggled;
};

void OptionManager::registerCategory(std::string_view category) {
    std::lock_guard<std::mutex> lk(m_mutex);
    if (std::find(m_categories.begin(), m_categories.end(), category) == m_categories.end()) m_categories.push_back(std::string(category));
};

void OptionManager::registerOption(const Option& option) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_options.push_back(option);

    registerCategory(option.category);
};

std::vector<Option> OptionManager::getOptions() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_options;
};

std::vector<std::string> OptionManager::getCategories() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_categories;
};

bool OptionManager::getOption(std::string_view id) const {
    return Mod::get()->getSavedValue<bool>(std::string(id), false);
};

bool OptionManager::setOption(std::string_view id, bool enable) const {
    return Mod::get()->setSavedValue(std::string(id), enable);
};

OptionManager* OptionManager::get() {
    static OptionManager inst;
    return &inst;
};