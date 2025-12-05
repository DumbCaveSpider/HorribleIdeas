#pragma once

#include <Horrible.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;
// Event for option toggles
class CategoryEvent : public Event {
protected:
    std::string m_id;

public:
    CategoryEvent(std::string id);

    std::string getId() const;
};

// Filter for option toggle event
class CategoryEventFilter : public EventFilter<CategoryEvent> {
public:
    using Callback = ListenerResult(CategoryEvent*);
    ListenerResult handle(std::function<Callback> fn, CategoryEvent* event);
};

class CategoryItem : public CCMenu {
protected:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    EventListener<CategoryEventFilter> m_listener = {
        [this](CategoryEvent* event) {
            return OnCategory(event->getId());
        },
        CategoryEventFilter()
    };

    CategoryItem();
    virtual ~CategoryItem();

    ListenerResult OnCategory(const std::string& category);

    void onToggle(CCObject* sender);

    bool init(CCSize const& size, const std::string& category);

public:
    static CategoryItem* create(CCSize const& size, const std::string& category);
};