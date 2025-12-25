#include "../SpamJumps.hpp"

#include <Utils.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace horrible;

class SpamJumps::Impl final {
public:
    int m_inputCount = 0;
    int m_inputTarget = 10;

    Ref<CCLabelBMFont> m_counter = nullptr;
    Ref<ProgressBar> m_timer = nullptr;

    std::function<void(bool)> m_callback;
};

SpamJumps::SpamJumps() {
    m_impl = std::make_unique<Impl>();
};

SpamJumps::~SpamJumps() {};

bool SpamJumps::init() {
    if (!CCBlockLayer::init()) return false;

    setID("spam-jumps"_spr);

    return true;
};

void SpamJumps::setCallback(std::function<void(bool)> cb) {
    m_impl->m_callback = cb;
};

bool SpamJumps::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    m_impl->m_inputCount++;
    if (m_impl->m_inputCount >= m_impl->m_inputTarget) if (m_impl->m_callback) m_impl->m_callback(true);

    return false;
};

SpamJumps* SpamJumps::create() {
    auto ret = new SpamJumps();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};