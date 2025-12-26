#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace horrible {
    // Popup that displays a random level thumbnail
    class RandomAd : public Popup<> {
    protected:
        void onPlayBtn(CCObject*);

        bool setup() override;

    public:
        static RandomAd* create();
    };
};