#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(AdvertsPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("ads");

        Ref<RandomAdPopup> m_ad = nullptr;
    };

    void setupHasCompleted() {
        nextAd();
        PlayLayer::setupHasCompleted();
    };

    void nextAd() {
        if (m_fields->enabled) scheduleOnce(schedule_selector(AdvertsPlayLayer::showAd), randng::get(15.f, 5.f));
    };

    void showAd(float) {
        if (m_fields->enabled) {
            nextAd(); // reschedule

            if (m_fields->m_ad) {
                m_fields->m_ad->removeMeAndCleanup();
                m_fields->m_ad = nullptr;
            };

#ifdef GEODE_IS_WINDOWS
            // Show cursor when ad appears
            CCEGLView::sharedOpenGLView()->showCursor(true);
#endif
            if (auto popup = RandomAdPopup::create()) {
                m_fields->m_ad = popup;
                m_fields->m_ad->show();
            };
        };
    };
};