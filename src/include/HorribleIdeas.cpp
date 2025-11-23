#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horribleideas;

HorribleOptionEvent::HorribleOptionEvent(std::string id, bool toggled)
    : m_id(id), m_toggled(toggled) {};

std::string HorribleOptionEvent::getId() const {
    return m_id;
};

bool HorribleOptionEvent::getToggled() const {
    return m_toggled;
};

HorribleOptionEventFilter::HorribleOptionEventFilter(std::string id) : m_id(id) {};

ListenerResult HorribleOptionEventFilter::handle(std::function<Callback> fn, HorribleOptionEvent* event) {
    if (event->getId() == m_id) return fn(event);
    return ListenerResult::Propagate;
};

OptionManager::OptionManager() {
    retain();
};

void OptionManager::registerCategory(std::string_view category) {
    if (std::find(m_categories.begin(), m_categories.end(), category) == m_categories.end()) m_categories.push_back(std::string(category));
};

void OptionManager::registerOption(const Option& option) {
    m_options.push_back(option);
    registerCategory(option.category);
};

std::vector<Option> OptionManager::getOptions() const {
    return m_options;
};

std::vector<std::string> OptionManager::getCategories() const {
    return m_categories;
};

bool OptionManager::getOption(std::string_view id) const {
    return Mod::get()->getSavedValue<bool>(std::string(id), false);
};

bool OptionManager::setOption(std::string_view id, bool enable) const {
    auto event = new HorribleOptionEvent(std::string(id), enable);
    event->postFromMod(Mod::get());

    return Mod::get()->setSavedValue(std::string(id), enable);
};

OptionManager* OptionManager::get() {
    static auto inst = new OptionManager();
    return inst;
};