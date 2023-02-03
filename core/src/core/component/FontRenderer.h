#pragma once
#include "_Core.h"

#include "generic/Component.h"

namespace core {

    class FontRenderer : public Component {
    private:

    public:
        FontRenderer();
        virtual ~FontRenderer();
        void start() override;
        void update(float dt) override;
        void stop() override { }
    };

}