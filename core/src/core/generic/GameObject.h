#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "generic/Transform.h"
#include "utils/DataPool.h"

namespace core {
    class GameObject {
    private:
        std::string name;
        std::vector<Component*> components;
        int zIndex;

        core_id objectID;

    public:
        Transform transform;
        GameObject(std::string name);
        GameObject(std::string name, Transform transform);
        GameObject(std::string name, Transform transform, DataPool::DISPLAYMODE displaymode);
        ~GameObject();

        Component* getComponent(std::string componentTypeID);
        bool removeComponent(Component* component);
        bool addComponent(Component* component);

        void update(float dt);
        void start();
        void imgui(float dt);

        void deleteComponents();
        std::string getName();
        int getZIndex();
        void setZIndex(int zIndex);
        core_id GetObjectID() const { return objectID; }

        DataPool::DISPLAYMODE displayMode;

        static std::unordered_map<Component*, GameObject*> CGMap;
    };

    
}