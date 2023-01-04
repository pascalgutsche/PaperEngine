#pragma once
#include "_Core.h"

#include "generic/Transform.h"
#include "utils/DataPool.h"

namespace core {

    // this is required in order for the compiler to find the class later on
    class GameObject;
    class Component;

    class Component {
    public:
        std::string getTypeID();
    protected:
        std::string typeID;
    public:
        Component();
        ~Component();
        GameObject* gameObject;
        virtual void start();
        virtual void update(float deltaTime);
        virtual void imgui(float deltaTime);
    };

    class GameObject {
    private:
        std::string name;
        std::vector<Component*> components;
        int zIndex;

    public:
        Transform transform;
        GameObject(std::string name);
        GameObject(std::string name, Transform transform);
        GameObject(std::string name, Transform transform, DataPool::DISPLAYMODE displaymode);

        Component* getComponent(std::string componentTypeID);
        bool removeComponent(Component* component);
        bool addComponent(Component* component);

        void update(float deltaTime);
        void start();
        void imgui(float deltaTime);

        void deleteComponents();
        std::string getName();
        int getZIndex();
        void setZIndex(int zIndex);

        DataPool::DISPLAYMODE displayMode;
    };

}