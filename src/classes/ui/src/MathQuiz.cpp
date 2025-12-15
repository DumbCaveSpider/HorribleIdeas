#include "../MathQuiz.hpp"

#include <Utils.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class MathQuiz::Impl final {
public:
    int m_numFirst = 0;
    int m_numSecond = 0;

    MathOperation m_operation = MathOperation::Addition;

    int m_correctAnswer = 0;
    std::vector<int> m_answers; // 4 answer options

    Ref<ProgressBar> m_timerBar = nullptr;
    Ref<CCMenu> m_answerMenu = nullptr;
    Ref<Richard> m_richard = nullptr;
    Ref<CCDrawNode> m_drawNode = nullptr;

    float m_timeRemaining = 10.f;
    float m_totalTime = 10.f;
    float m_timeDt = 0.f;

    std::function<void()> m_onCloseCallback = nullptr;
    bool m_wasCorrect = false;
};

MathQuiz::MathQuiz() {
    m_impl = std::make_unique<Impl>();
};

MathQuiz::~MathQuiz() {};

bool MathQuiz::init() {
    if (!CCBlockLayer::init()) return false;

    setID("math-quiz"_spr);

    m_impl->m_numFirst = randng::get(10);
    m_impl->m_numSecond = randng::get(10);

    m_impl->m_operation = static_cast<MathOperation>(randng::get(3));
    switch (m_impl->m_operation) {
    case MathOperation::Addition:
        m_impl->m_correctAnswer = m_impl->m_numFirst + m_impl->m_numSecond;
        break;

    case MathOperation::Subtraction:
        m_impl->m_correctAnswer = m_impl->m_numFirst - m_impl->m_numSecond;
        break;

    case MathOperation::Multiplication:
        m_impl->m_correctAnswer = m_impl->m_numFirst * m_impl->m_numSecond;
        break;

    case MathOperation::Geometry:
        break;
    };

    auto winSize = CCDirector::get()->getWinSize();
    // geometry dash
    if (m_impl->m_operation == MathOperation::Geometry) {
        int sides = randng::get(10);
        m_impl->m_correctAnswer = sides;

        // draw node and polygon points
        m_impl->m_drawNode = CCDrawNode::create();
        m_impl->m_drawNode->setID("math-quiz-drawnode");

        float radius = std::min(winSize.width, winSize.height) / 8.f;
        float centerX = winSize.width / 2.f - 160.f;
        float centerY = winSize.height / 2.f;

        std::vector<CCPoint> polyPoints;
        polyPoints.reserve(sides);

        constexpr float PI = 3.14159265358979323846f;
        float theta = (2.f * PI) / sides;

        for (int i = 0; i < sides; ++i) {
            float angle = theta * i - PI / 2.f;  // start at top
            float x = radius * cosf(angle);
            float y = radius * sinf(angle);
            polyPoints.push_back(ccp(x, y));
        };

        // draw the polygon in local coords with drawNode placed at center
        m_impl->m_drawNode->setPosition({ centerX, centerY });

        cocos2d::ccColor4F fillColor = { 0.85f, 0.65f, 0.15f, 1.f };
        cocos2d::ccColor4F borderColor = { 0.05f, 0.05f, 0.05f, 1.f };

        m_impl->m_drawNode->clear();
        m_impl->m_drawNode->drawPolygon(polyPoints.data(), static_cast<unsigned int>(polyPoints.size()), fillColor, 2.f, borderColor);

        addChild(m_impl->m_drawNode, 250);
    };

    // Create problem label
    std::string problemText;
    if (m_impl->m_operation == MathOperation::Geometry) {
        problemText = "How many sides does this shape have?";
    } else {
        std::string operationSymbol = (m_impl->m_operation == MathOperation::Addition) ? "+" : (m_impl->m_operation == MathOperation::Subtraction) ? "-"
            : "*";
        problemText = fmt::format("{} {} {}", m_impl->m_numFirst, operationSymbol, m_impl->m_numSecond);
    };

    // reuse winSize declared above
    auto problemLabel = CCLabelBMFont::create(problemText.c_str(), "bigFont.fnt");
    problemLabel->setID("problem-label");
    problemLabel->setPosition({ winSize.width / 2.f, winSize.height - 60.f });
    problemLabel->setScale(0.9f);

    addChild(problemLabel);

    if (m_impl->m_operation != MathOperation::Geometry) {
        auto equalsLabel = CCLabelBMFont::create("= ?", "bigFont.fnt");
        equalsLabel->setID("equals-label");
        equalsLabel->setPosition({ winSize.width / 2.f, winSize.height - 100.f });

        addChild(equalsLabel);
    };

    // make the text smaller for geometry
    if (m_impl->m_operation == MathOperation::Geometry) problemLabel->setScale(0.5f);

    // i hope i did this right cheese, u added this progress bar thing
    m_impl->m_timerBar = ProgressBar::create();
    m_impl->m_timerBar->setID("math-quiz-timer");
    m_impl->m_timerBar->setStyle(ProgressBarStyle::Solid);
    m_impl->m_timerBar->setFillColor({ 255, 210, 0 });
    m_impl->m_timerBar->setAnchorPoint({ 0.5, 0.5 });
    m_impl->m_timerBar->setPosition({ winSize.width / 2.f, winSize.height - 20.f });
    m_impl->m_timerBar->setZOrder(200);

    addChild(m_impl->m_timerBar);

    m_impl->m_timeRemaining = m_impl->m_totalTime = 10.f;
    m_impl->m_timerBar->updateProgress(100.f);

    scheduleUpdate();

    // Generate 4 answer options with the correct answer randomized
    m_impl->m_answers.clear();
    m_impl->m_answers.push_back(m_impl->m_correctAnswer);

    // Add 3 wrong answers
    if (m_impl->m_operation == MathOperation::Geometry) {
        while (m_impl->m_answers.size() < 4) {
            int wrongAnswer = randng::get(10);
            if (wrongAnswer != m_impl->m_correctAnswer && std::find(m_impl->m_answers.begin(), m_impl->m_answers.end(), wrongAnswer) == m_impl->m_answers.end()) m_impl->m_answers.push_back(wrongAnswer);
        };
    } else {
        while (m_impl->m_answers.size() < 4) {
            int wrongAnswer = m_impl->m_correctAnswer + randng::get(10);
            if (wrongAnswer != m_impl->m_correctAnswer && std::find(m_impl->m_answers.begin(), m_impl->m_answers.end(), wrongAnswer) == m_impl->m_answers.end()) m_impl->m_answers.push_back(wrongAnswer);
        };
    };

    // Shuffle the answers
    utils::random::shuffle<std::vector<int>>(m_impl->m_answers);

    // richard floating lol
    if (auto richard = Richard::create()) {
        m_impl->m_richard = richard;
        m_impl->m_richard->setID("richard");
        m_impl->m_richard->setAnchorPoint({ 1, 0.5 });
        m_impl->m_richard->setScale(0.6f);
        m_impl->m_richard->setPosition({ winSize.width - 36.f, winSize.height / 2.f });
        m_impl->m_richard->setZOrder(500);

        addChild(m_impl->m_richard);

        // floating animation
        float floatDistance = 8.f;
        float floatTime = 1.f;

        auto moveUp = CCMoveBy::create(floatTime, ccp(0, floatDistance));
        auto moveDown = CCMoveBy::create(floatTime, ccp(0, -floatDistance));

        auto seq = CCSequence::create(moveUp, moveDown, nullptr);
        auto repeat = CCRepeatForever::create(seq);

        m_impl->m_richard->runAction(repeat);
    };

    m_impl->m_answerMenu = CCMenu::create();
    m_impl->m_answerMenu->setID("answer-menu");
    m_impl->m_answerMenu->setPosition({ winSize.width / 2.f, winSize.height / 2.f - 20.f });

    float buttonWidth = 80.f;
    float buttonHeight = 35.f;
    float spacingX = 100.f;
    float spacingY = 50.f;

    for (int i = 0; i < 4; i++) {
        auto btnSprite = ButtonSprite::create(
            fmt::format("{}", m_impl->m_answers[i]).c_str(),
            buttonWidth,
            true,
            "bigFont.fnt",
            "GJ_button_01.png",
            0,
            0.8f);

        //  2x2 grid
        float x = (i % 2 == 0) ? -spacingX / 2.f : spacingX / 2.f;
        float y = (i < 2) ? spacingY / 2.f : -spacingY / 2.f;

        auto answerBtn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(MathQuiz::onAnswerClicked)
        );
        answerBtn->setID("submit-answer-btn");
        answerBtn->setPosition({ x, y });
        answerBtn->setTag(m_impl->m_answers[i]);

        m_impl->m_answerMenu->addChild(answerBtn);
    };

    addChild(m_impl->m_answerMenu);

    // @geode-ignore(unknown-resource)
    if (auto fmod = FMODAudioEngine::sharedEngine()) fmod->playEffectAsync("chest07.ogg");

    return true;
};

void MathQuiz::setOnCloseCallback(std::function<void()> cb) {
    m_impl->m_onCloseCallback = cb;
};

void MathQuiz::setWasCorrectFlag(bool v) {
    m_impl->m_wasCorrect = v;
    // @geode-ignore(unknown-resource)
    if (auto fmod = FMODAudioEngine::sharedEngine()) fmod->playEffectAsync("crystal01.ogg");
};

bool MathQuiz::wasCorrect() const {
    return m_impl->m_wasCorrect;
};

void MathQuiz::onAnswerClicked(CCObject* sender) {
    if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)) {
        int selectedAnswer = btn->getTag();
        bool correct = (selectedAnswer == m_impl->m_correctAnswer);

        // Button shake for incorrect fancy
        if (!correct) {
            auto shake = CCSequence::create(
                CCMoveBy::create(0.03f, ccp(-6, 0)),
                CCMoveBy::create(0.06f, ccp(12, 0)),
                CCMoveBy::create(0.03f, ccp(-6, 0)),
                nullptr);
            btn->runAction(shake);
        };

        if (m_impl->m_answerMenu) m_impl->m_answerMenu->removeFromParentAndCleanup(true);
        if (m_impl->m_drawNode) m_impl->m_drawNode->removeFromParentAndCleanup(true);

        // feedback label
        // Notification::create(correct ? "Correct!" : "Incorrect!", correct ? NotificationIcon::Success : NotificationIcon::Error, 1.5f)->show();
        auto winSize = CCDirector::get()->getWinSize();
        auto feedbackLabel = CCLabelBMFont::create(correct ? "Correct!" : "Incorrect!", "goldFont.fnt");
        feedbackLabel->setID("feedback-label");
        feedbackLabel->setAnchorPoint({ 0.5, 0.5 });
        feedbackLabel->setPosition({ winSize.width / 2.f, winSize.height / 2.f });
        feedbackLabel->setScale(0.1f);

        if (!correct) {
            feedbackLabel->setColor({ 255, 100, 100 });
        } else {
            feedbackLabel->setColor({ 100, 255, 100 });
        };

        addChild(feedbackLabel, 1000);

        // Small scale animation, delay, then call close
        auto seq = CCSequence::create(
            CCScaleTo::create(0.12f, 1.2f),
            CCScaleTo::create(0.08f, 1.f),
            CCDelayTime::create(0.75f),
            CCCallFuncN::create(this, callfuncN_selector(MathQuiz::closeAfterFeedback)),
            nullptr);
        feedbackLabel->runAction(seq);

        setKeypadEnabled(false);
        setWasCorrectFlag(correct);
        unscheduleUpdate();
    };
};

void MathQuiz::keyBackClicked() {
    if (m_impl->m_answerMenu) m_impl->m_answerMenu->removeFromParentAndCleanup(true);
    if (m_impl->m_drawNode) m_impl->m_drawNode->removeFromParentAndCleanup(true);

    Notification::create("Nope! you cant escape the math quiz!", NotificationIcon::Error, 1.5f)->show();

    setWasCorrectFlag(false);
    closePopup();
};

void MathQuiz::closeAfterFeedback(CCNode* node) {
    if (node) node->removeFromParentAndCleanup(true);

    if (m_impl->m_answerMenu) m_impl->m_answerMenu->removeFromParentAndCleanup(true);
    if (m_impl->m_drawNode) m_impl->m_drawNode->removeFromParentAndCleanup(true);

    if (m_impl->m_onCloseCallback) m_impl->m_onCloseCallback();

    unscheduleUpdate();
    removeFromParentAndCleanup(true);
};

void MathQuiz::closePopup() {
    if (m_impl->m_answerMenu) m_impl->m_answerMenu->removeFromParentAndCleanup(true);
    if (m_impl->m_onCloseCallback) m_impl->m_onCloseCallback();

    unscheduleUpdate();
    removeFromParentAndCleanup(true);
};

void MathQuiz::update(float dt) {
    if (m_impl->m_timeRemaining <= 0.f) return;
    m_impl->m_timeRemaining -= dt;

    m_impl->m_timeDt += dt;
    if (m_impl->m_timeDt >= 0.5f) {
        // @geode-ignore(unknown-resource)
        if (auto fmod = FMODAudioEngine::sharedEngine()) fmod->playEffectAsync("counter003.ogg");
        m_impl->m_timeDt = 0.f;
    };

    if (m_impl->m_timeRemaining < 0.f) m_impl->m_timeRemaining = 0.f;
    float pct = (m_impl->m_timeRemaining / m_impl->m_totalTime) * 100.f;

    if (m_impl->m_timerBar) m_impl->m_timerBar->updateProgress(pct);

    if (m_impl->m_timeRemaining <= 0.f) {
        // automatic incorrect
        unscheduleUpdate();
        setWasCorrectFlag(false);

        // Notification::create("Time's Up!", NotificationIcon::Error, 1.5f)->show();

        auto winSize = CCDirector::get()->getWinSize();

        if (m_impl->m_answerMenu) m_impl->m_answerMenu->removeFromParentAndCleanup(true);

        auto feedbackLabel = CCLabelBMFont::create("Time's Up!", "goldFont.fnt");
        feedbackLabel->setID("feedback-label");
        feedbackLabel->setAnchorPoint({ 0.5, 0.5 });
        feedbackLabel->setPosition({ winSize.width / 2.f, winSize.height / 2.f });
        feedbackLabel->setScale(0.1f);
        feedbackLabel->setColor({ 255, 100, 100 });
        addChild(feedbackLabel, 1000);

        auto seq = CCSequence::create(
            CCScaleTo::create(0.12f, 1.2f),
            CCScaleTo::create(0.08f, 1.f),
            CCDelayTime::create(0.75f),
            CCCallFuncN::create(this, callfuncN_selector(MathQuiz::closeAfterFeedback)),
            nullptr);

        feedbackLabel->runAction(seq);
    };
};

MathQuiz* MathQuiz::create() {
    auto ret = new MathQuiz();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};

class Richard::Impl final {
public:
    Ref<CCSprite> m_sprite = nullptr;
};

Richard::Richard() {
    m_impl = std::make_unique<Impl>();
};

Richard::~Richard() {};

bool Richard::init() {
    if (!CCNode::init()) return false;

    m_impl->m_sprite = CCSprite::createWithSpriteFrameName("diffIcon_02_btn_001.png");
    m_impl->m_sprite->setID("richard-sprite");
    m_impl->m_sprite->setAnchorPoint({ 0.5f, 0.5f });
    m_impl->m_sprite->setScale(5.f);

    addChild(m_impl->m_sprite);

    setContentSize(m_impl->m_sprite->getScaledContentSize());
    m_impl->m_sprite->setPosition({ getScaledContentWidth() / 2.f, getScaledContentHeight() / 2.f });

    return true;
};

Richard* Richard::create() {
    auto ret = new Richard();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};