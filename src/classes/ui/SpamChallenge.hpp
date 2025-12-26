#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    class SpamChallenge : public CCBlockLayer, public FLAlertLayerProtocol {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        SpamChallenge();
        virtual ~SpamChallenge();

        void closeAfterFeedback(float);
        void setSuccess(bool v);

        void keyBackClicked() override;
        void update(float dt) override;

        bool init() override;

    public:
        static SpamChallenge* create();

        bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;

        void setCallback(std::function<void(bool)> const& cb);
    };
};