#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "generic/Transform.h"
#include "utils/DataPool.h"

namespace core {
    class CORE_API GameObject {
    private:
        std::string id;
        std::vector<Component*> components;
        int zIndex;

    public:
        Transform transform;
        GameObject(std::string id);
        GameObject(std::string id, Transform transform);
        GameObject(std::string id, Transform transform, DataPool::DISPLAYMODE displaymode);
        ~GameObject();

        Component* getComponent(std::string componentTypeID);
        bool removeComponent(Component* component);
        bool addComponent(Component* component);

        void update(float dt);
        void start();
        void imgui(float dt);

        void deleteComponents();
        std::string getStringID();
        int getZIndex();
        void setZIndex(int zIndex);

        DataPool::DISPLAYMODE displayMode;

        static std::unordered_map<Component*, GameObject*> CGMap;
    };

    
}