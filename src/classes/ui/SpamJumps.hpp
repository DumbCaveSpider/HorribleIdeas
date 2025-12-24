#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    class SpamJumps : public CCBlockLayer, public FLAlertLayerProtocol {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        SpamJumps();
        virtual ~SpamJumps();

        bool init() override;

    public:
        static SpamJumps* create();

        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

        void setCallback(std::function<void(bool)> cb);
    };
};