#include <Horrible.hpp>
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(HorriblePlayLayer, PlayLayer)
{
    struct Fields
    {
        bool safeMode = horribleMod->getSettingValue<bool>("safe-mode");
    };

    // safe mode prevents level completion
    void levelComplete()
    {

        if (m_fields->safeMode)
        {
            bool testMode = m_isTestMode;

            m_isTestMode = true;
            m_isTestMode = testMode;
            PlayLayer::levelComplete();
            return;
        }

        PlayLayer::levelComplete();
    };
};