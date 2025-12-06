#include <Horrible.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace horrible;

class $modify(MockMenuLayer, MenuLayer) {
    struct Fields {
        bool enabled = options::get("mock");
        int chance = options::getChance("mock");
    };

    bool init() {
        if (!MenuLayer::init()) return false;

        int rnd = randng::fast();
        log::debug("mock chance {}", rnd);

        // Show a LazySprite for the first PNG found in the save directory
        if (m_fields->enabled) {
            log::debug("mock feature enabled in MainMenu layer");

            if (rnd <= m_fields->chance) {
                auto mockConfigPath = fmt::format("{}\\mock.json", horribleMod->getSaveDir());
                auto mockConfig = file::readJson(fs::path(mockConfigPath));

                log::debug("Reading path {}...", mockConfigPath);

                if (mockConfig.isOk()) {
                    log::debug("Read mocking config file");

                    auto mockConfigUnwr = mockConfig.unwrapOr(matjson::Value());

                    auto lvlUnwr = mockConfigUnwr.begin();
                    std::advance(lvlUnwr, rnd % mockConfigUnwr.size());

                    auto id = lvlUnwr->getKey().value_or("");
                    auto percent = lvlUnwr->asInt().unwrapOr(99);

                    if (!id.empty()) {
                        log::debug("ID {} with percentage {} is valid", id, percent);

                        std::string pngPath = fmt::format("{}\\{}.png", horribleMod->getSaveDir(), id);

                        log::info("Displaying {}", pngPath);

                        auto ss = LazySprite::create({ 192.f, 108.f });
                        ss->setID("mock"_spr);
                        ss->setScale(0.25);
                        ss->setAnchorPoint({ 0.5, 0.5 });
                        ss->setPosition({ -192.f, getScaledContentHeight() / 2.f });
                        ss->setZOrder(1000);

                        ss->setLoadCallback([this, ss, percent, rnd](Result<> res) {
                            if (res.isOk()) {
                                log::info("Sprite loaded successfully from save dir PNG");

                                auto percLabelText = fmt::format("{}%", percent);

                                auto percLabel = CCLabelBMFont::create(percLabelText.c_str(), "bigFont.fnt");
                                percLabel->setID("percentage");
                                percLabel->setPosition({ ss->getScaledContentWidth() / 2.f, ss->getScaledContentHeight() / 2.f });
                                percLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
                                percLabel->ignoreAnchorPointForPosition(false);
                                percLabel->setAnchorPoint({ 0, 0 });
                                percLabel->setScale(2.5);

                                ss->addChild(percLabel);

                                auto rA = static_cast<float>(randng::fast()) / 100.f;
                                auto rB = static_cast<float>(randng::fast()) / 100.f;

                                float yA = getScaledContentHeight() * rB; // starting height pos
                                float yB = getScaledContentHeight() * rA; // ending height pos

                                ss->setPositionY(getScaledContentHeight() * yA);
                                ss->setRotation(360.f * (yA * yB)); // random rotation

                                auto move = CCEaseIn::create(CCMoveTo::create(10.f, { getScaledContentWidth() + 192.f, getScaledContentHeight() * yB }), 1.f);
                                auto rotate = CCEaseOut::create(CCRotateBy::create(12.5f, 45.f), 1.f);

                                auto action = CCSpawn::createWithTwoActions(move, rotate);
                                ss->runAction(action);

                                log::info("Animated sprite successfully");
                            } else {
                                log::error("Sprite failed to load: {}", res.unwrapErr());
                                ss->removeMeAndCleanup();
                            }; });

                            ss->loadFromFile(fs::path(pngPath));
                            addChild(ss);
                    } else {
                        log::error("ID is invalid");
                    };
                } else {
                    log::error("Mocking data file not found");
                };
            };
        };

        return true;
    };
};

class $modify(MockPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = options::get("mock");
    };

    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
        int id = m_level->m_levelID;
        int percentage = m_level->m_normalPercent;

        log::info("Showing new best for level ID: {}", id);
        log::info("Level percentage: {}", percentage);

#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS) // not available for these platforms
        if (m_fields->enabled && percentage >= 90) {
            CCDirector* director = CCDirector::sharedDirector();
            CCScene* scene = CCScene::get();

            // Get the window size in points and scale to pixels
            auto winSize = director->getWinSize();

            int width = static_cast<int>(winSize.width);
            int height = static_cast<int>(winSize.height);

            CCRenderTexture* renderTexture = CCRenderTexture::create(width, height);

            renderTexture->begin();
            scene->visit();
            renderTexture->end();

            if (auto image = renderTexture->newCCImage()) {
                std::string path = fmt::format("{}\\{}.png", horribleMod->getSaveDir(), id);

                if (image->saveToFile(path.c_str(), false)) {
                    auto mockConfigPath = fmt::format("{}\\mock.json", horribleMod->getSaveDir());
                    auto mockConfig = file::readJson(std::filesystem::path(mockConfigPath)); // get the saved fails to mock the player with :)

                    auto toWrite = matjson::Value(); // what we're gonna write in the mock.json file

                    if (mockConfig.isOk()) {
                        // unwrap the whole thing
                        auto mockConfigUnwr = mockConfig.unwrapOr(matjson::Value());

                        // overwrite this field (or add it) with the percent
                        mockConfigUnwr[std::to_string(id)] = percentage;

                        toWrite = mockConfigUnwr;
                    } else {
                        toWrite = matjson::makeObject({ {std::to_string(id), percentage} });
                    };

                    if (!toWrite.isNull()) {
                        auto mockJson = file::writeToJson(mockConfigPath, toWrite);

                        if (mockJson.isOk()) {
                            log::info("Saved highly mockable percentage of {} to data", percentage);
                        } else {
                            log::error("Aw man, failed to save mockable percentage of {} to data", percentage);
                        };
                    };

                    log::info("Saved screenshot to {}", path);
                } else {
                    log::error("Failed to save screenshot to {}", path);
                };

                CC_SAFE_DELETE(image);
            } else {
                log::error("Failed to create image from render texture");
            };
        };
#endif

#if defined(GEODE_IS_MACOS) && defined(GEODE_IS_IOS)
        Notification::create("Mock is not avaliable on this platform", NotificationIcon::Info, 1.f);
#endif

        PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
    };

    void levelComplete() {
        if (m_fields->enabled) {
            int id = m_level->m_levelID;
            int percentage = m_level->m_normalPercent;

            auto mockConfigPath = fmt::format("{}\\mock.json", horribleMod->getSaveDir());
            auto mockConfig = file::readJson(std::filesystem::path(mockConfigPath)); // get the saved levels to mock the player :)

            if (mockConfig.isOk()) {
                log::debug("Clearing mock record for {}", id);
                auto mockConfigUnwr = mockConfig.unwrapOr(matjson::Value());

                mockConfigUnwr[std::to_string(id)].clear();

                auto mockJson = file::writeToJson(mockConfigPath, mockConfigUnwr);

                if (mockJson.isOk()) {
                    log::info("Saved highly mockable percentage of {} to data", percentage);
                } else {
                    log::error("Aw man, failed to save mockable percentage of {} to data", percentage);
                };
            };
        };

        PlayLayer::levelComplete();
    };
};