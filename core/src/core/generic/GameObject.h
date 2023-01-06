#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "generic/Transform.h"
#include "utils/DataPool.h"

namespace core {
    class CORE_API GameObject {
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

        static std::unordered_map<Component*, GameObject*> CGMap;
    };

    
}