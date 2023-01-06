#pragma once
#include "_Core.h"
#include "utility.h"


namespace core {

    

    class CORE_API Component {
    public:
        std::string getTypeID();
    protected:
        std::string typeID;
    public:
        Component();
        ~Component();
        virtual void start();
        virtual void update(float deltaTime);
        virtual void imgui(float deltaTime);
    };

}