#include <ctime>
#include <cstdlib>

#include <Geode/Geode.hpp>

#include <Geode/utils/terminate.hpp>

#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

struct RandomSeeder
{
    RandomSeeder() { srand(time(0)); }
};
static RandomSeeder _randomSeeder;

class $modify(HorriblePlayerObject, PlayerObject)
{
    void updateJump(float p0)
    {
        auto horribleMod = getMod();
        auto rnd = rand() % 101;
        // log::debug("player object chance {}", rnd);

        if (horribleMod->getSavedValue<bool>("gravity", false))
        {
            float newGrav = std::round((static_cast<float>(rnd) / 100.f) * (2.5f - 0.01f) * 100.0f) / 100.0f;

            // check if player touched any floor i think
            auto onGrnd = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4;

            onGrnd ? (m_gravityMod = 1.f * newGrav) : (m_gravityMod = 1.f);
            if (onGrnd && !m_isRotating)
                log::debug("set gravity to x{}", newGrav);
        };

        PlayerObject::updateJump(p0);
    };

    void update(float p0)
    {
        auto playLayer = PlayLayer::get();
        auto horribleMod = getMod();

        auto healthEnabled = horribleMod->getSavedValue<bool>("health", false);

        if (playLayer && playLayer->m_percentageLabel)
        {
            // lil hack on getting the percentage lmao
            std::string percentage = playLayer->m_percentageLabel->getString();
            percentage.erase(std::remove(percentage.begin(), percentage.end(), '%'), percentage.end());
            int currentPercentage = std::stoi(percentage);
            // log::debug("percentage {}", currentPercentage);

            if (horribleMod->getSavedValue<bool>("freeze", false))
            {
                // check if current percentage is less than or equal to 90
                if (currentPercentage >= 90)
                {
                    auto gm = GameManager::get();
                    float oldFPS = Mod::get()->getSavedValue<float>("fps");

                    gm->setGameVariable("0116", true);

                    // Randomize FPS between 1 and 30
                    int randomFPS = 1 + (rand() % 31); // 1 to 30 inclusive
                    float interval = 1.f / static_cast<float>(randomFPS);
                    if (interval <= 0.0f || interval > 1.0f)
                        interval = 1.f / 60.f; // fallback to 60 FPS if invalid
                    CCDirector::sharedDirector()->setAnimationInterval(interval);
                    // log::debug("cap fps to {} (interval {})", randomFPS, interval);
                }
                else
                {
                    // default to user old fps
                    auto gm = GameManager::get();
                    float oldFPS = Mod::get()->getSavedValue<float>("fps");
                    gm->setGameVariable("0116", true);

                    // Use seconds per frame, not raw FPS
                    float interval = (oldFPS > 10.f) ? (1.f / oldFPS) : (1.f / 60.f); // minimum 10 FPS
                    if (interval <= 0.0f || interval > 1.0f)
                        interval = 1.f / 60.f; // fallback to 60 FPS if invalid
                    CCDirector::sharedDirector()->setAnimationInterval(interval);
                    // log::debug("reset fps to {} (interval {})", oldFPS, interval);
                }
            }
        }
        PlayerObject::update(p0);
    }
};
