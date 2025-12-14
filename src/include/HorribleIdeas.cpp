#define GEODE_DEFINE_EVENT_EXPORTS
#include <HorribleIdeas.hpp>
#include <OptionalAPI.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horribleideas;

HorribleOptionEvent::HorribleOptionEvent(std::string id, bool toggled) : m_id(id), m_toggled(toggled) {};

std::string HorribleOptionEvent::getId() const {
    return m_id;
};

bool HorribleOptionEvent::getToggled() const {
    return m_toggled;
};

HorribleOptionEventFilter::HorribleOptionEventFilter(const std::string& id) : m_ids({ id }) {};
HorribleOptionEventFilter::HorribleOptionEventFilter(std::vector<std::string> ids) : m_ids(ids) {};

ListenerResult HorribleOptionEventFilter::handle(std::function<Callback> fn, HorribleOptionEvent* event) {
    if (std::find(m_ids.begin(), m_ids.end(), event->getId()) == m_ids.end()) return fn(event);
    return ListenerResult::Propagate;
};

class OptionManager::Impl final {
public:
    std::vector<Option> m_options = {}; // Array of registered options
    std::vector<std::string> m_categories = {}; // Array of auto-registered categories
};

OptionManager::OptionManager() {
    m_impl = std::make_unique<Impl>();
    retain();
};

OptionManager::~OptionManager() {};

void OptionManager::registerCategory(const std::string& category) {
    if (!utils::string::containsAny(category, getCategories())) m_impl->m_categories.push_back(std::string(category));
};

bool OptionManager::doesOptionExist(std::string_view id) const {
    for (const auto& option : getOptions()) {
        if (option.id == id) return true;
    };

    return false;
};

void OptionManager::registerOption(const Option& option) {
    if (doesOptionExist(option.id)) {
        log::error("Could not register option '{}' ({}) because it already exists!", option.name, option.id);
    } else {
        m_impl->m_options.push_back(option);
        registerCategory(option.category);

        log::debug("Registered option {}", option.id);
    };
};

std::vector<Option> OptionManager::getOptions() const {
    return m_impl->m_options;
};

std::vector<std::string> OptionManager::getCategories() const {
    return m_impl->m_categories;
};

bool OptionManager::getOption(std::string_view id) const {
    return Mod::get()->getSavedValue<bool>(std::string(id), false);
};

bool OptionManager::setOption(const std::string& id, bool enable) const {
    auto event = new HorribleOptionEvent(id, enable);
    event->postFromMod(Mod::get());

    auto eventV2 = new HorribleOptionEventV2(id, enable);
    eventV2->postFromMod(Mod::get());

    return Mod::get()->setSavedValue(std::string(id), enable);
};

OptionManager* OptionManager::get() {
    static auto inst = new OptionManager();
    return inst;
};

Result<> OptionManagerV2::registerOption(const Option& option) {
    if (auto om = OptionManager::get()) om->registerOption(option);
    return Ok();
};

Result<bool> OptionManagerV2::getOption(std::string_view id) {
    if (auto om = OptionManager::get()) return Ok(om->getOption(id));
    return Err("Failed to get OptionManager");
};

Result<bool> OptionManagerV2::setOption(const std::string& id, bool enable) {
    if (auto om = OptionManager::get()) return Ok(om->setOption(id, enable));
    return Err("Failed to get OptionManager");
};