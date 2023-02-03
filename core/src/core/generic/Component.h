#pragma once
#include "_Core.h"
#include "utility.h"


namespace core {

    class CORE_API Component {
    private:
        std::string typeID;
    public:
        Component(std::string typeID)
	        :typeID(typeID) { }

        virtual ~Component() = default;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void update(float dt) = 0;
        virtual void imgui(float dt) { }

        std::string getTypeID()
        {
            return this->typeID;
        }
    };

}