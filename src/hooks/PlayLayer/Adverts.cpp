#include <Utils.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace horrible;

class $modify(AdvertsPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("ads");

        Ref<RandomAd> m_ad = nullptr;
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();
        nextAd();
    };

    void nextAd() {
        auto delay = randng::get(15.f, 5.f);
        log::debug("scheduling ad in {}s", delay);

        if (m_fields->enabled) scheduleOnce(schedule_selector(AdvertsPlayLayer::showAd), delay);
    };

    void showAd(float) {
        if (m_fields->enabled) {

            if (m_fields->m_ad) {
                m_fields->m_ad->removeMeAndCleanup();
                m_fields->m_ad = nullptr;
            };

#ifdef GEODE_IS_WINDOWS
            // Show cursor when ad appears
            CCEGLView::sharedOpenGLView()->showCursor(true);
#endif
            if (auto popup = RandomAd::create()) {
                m_fields->m_ad = popup;
                m_fields->m_ad->show();
            };
        };

        nextAd();
    };
};