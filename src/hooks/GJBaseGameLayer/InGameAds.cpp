#include <Utils.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(AdsGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        bool enabled = options::get("ads");

        RandomAdPopup* m_currentAd = nullptr;
        float m_adCooldown = 0.f;
    };

    void update(float p0) {
        if (auto playLayer = PlayLayer::get()) {
            if (m_fields->enabled) {
                m_fields->m_adCooldown += p0;

                static float nextAdTime = 0.f;
                int rnd = randng::fast();
                auto gm = GameManager::sharedState();

                if (nextAdTime == 0.f || m_fields->m_adCooldown >= nextAdTime) {
                    // Randomize next ad time between 20 and 40 seconds
                    nextAdTime = 20.f + static_cast<float>(rnd % 21); // 20 to 40 inclusive
                    m_fields->m_adCooldown = 0.f;

                    log::warn("ad time!");

#ifdef GEODE_IS_WINDOWS
                    // Show cursor when ad appears
                    CCEGLView::sharedOpenGLView()->showCursor(true);
#endif
                    if (!m_isPracticeMode && !m_fields->m_currentAd) {
                        if (auto popup = RandomAdPopup::create()) {
                            m_fields->m_currentAd = popup;
                            m_fields->m_currentAd->show();
                        };
                    };

                    // if there's already an ad muahahahaha
                    if (auto alreadyPopup = CCScene::get()->getChildByID("ad"_spr)) {
                        log::debug("there's already an ad!");
                    } else {
                        m_fields->m_currentAd = nullptr;
                    };
                };

#ifdef GEODE_IS_WINDOWS
                // Hide cursor if ad popup is closed
                if (!m_fields->m_currentAd && !CCScene::get()->getChildByID("ad"_spr)) CCEGLView::sharedOpenGLView()->showCursor(false);
#endif
            };

            GJBaseGameLayer::update(p0);
        };
    };
};