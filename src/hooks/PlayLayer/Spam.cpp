#include <Utils.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace horrible;

class $modify(SpamPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("spam");
        int chance = options::getChance("spam");

        SpamChallenge* m_currentSpam = nullptr;
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();
        if (m_fields->enabled) nextSpam();
    };

    void nextSpam() {
        log::debug("scheduling spam challenge");
        if (m_fields->enabled && !m_hasCompletedLevel) scheduleOnce(schedule_selector(SpamPlayLayer::doSpam), randng::get(30.f, 5.f) * chanceToDelayPct(m_fields->chance));
    };

    void doSpam(float) {
        if (m_fields->enabled && !m_isPracticeMode) {
            log::info("Showing spam challenge");

            if (auto spam = SpamChallenge::create()) {
                m_fields->m_currentSpam = spam;

                // clear pointer on close / handle correct/wrong answer
                m_fields->m_currentSpam->setCallback([this](bool success) {
                    nextSpam();
                    if (!success) resetLevelFromStart();

                    if (m_fields->m_currentSpam) m_fields->m_currentSpam->removeMeAndCleanup();
                    m_fields->m_currentSpam = nullptr;
                                                     });

#ifdef GEODE_IS_WINDOWS
                CCEGLView::sharedOpenGLView()->showCursor(true);
#endif
                m_uiLayer->addChild(m_fields->m_currentSpam, 99);
            };
        } else if (m_fields->enabled) {
            nextSpam();
        };
    };

    void destroyPlayer(PlayerObject * player, GameObject * object) {
        PlayLayer::destroyPlayer(player, object);

        if (m_player1->m_isDead) {
            if (m_fields->m_currentSpam) {
                m_fields->m_currentSpam->removeMeAndCleanup();
                nextSpam();
            };

            m_fields->m_currentSpam = nullptr;
        };
    };
};