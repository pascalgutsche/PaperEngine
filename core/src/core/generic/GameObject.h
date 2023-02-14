#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Transform.h"
#include "utils/DataPool.h"
#include "event/Event.h"

namespace core {

    class Component;
    class SpriteRenderer;

    class GameObject {
    private:
        std::string name;
        std::vector<Component*> components;
        int zIndex;

        bool running = false;

        core_id objectID;

        static std::unordered_map<core_id, GameObject*> IDMap;
    public:
        Transform transform;
        GameObject(std::string name);
        GameObject(std::string name, Transform transform);
        GameObject(std::string name, Transform transform, DataPool::DISPLAYMODE displaymode);
        ~GameObject();

        template<typename T>
        T& GetComponent();
        Component* getComponent(std::string componentTypeID);

        template<typename T>
        bool RemoveComponent();
        bool removeComponent(Component* component);

        bool AddComponent(Component* component);

        void update(float dt);
        void start();
        void stop();
        void imgui(float dt);
        void event(Event& event);

        void deleteComponents();

        std::string getName();
        int getZIndex();
        void setZIndex(int zIndex);
        core_id GetObjectID() const { return objectID; }
        bool IsRunning() const { return running; }

        DataPool::DISPLAYMODE displayMode;

        static GameObject* GetGameObjectByID(core_id id);

        
    };

    template <typename T>
    T& GameObject::GetComponent()
    {
        T* derived = nullptr;
        for (Component* com : components)
        {
            derived = dynamic_cast<T*>(com);
        }
        return *derived;
    }

    template <typename T>
    bool GameObject::RemoveComponent() {
        for (int i = 0; i < components.size(); i++)
        {
            if (dynamic_cast<T*>(components[i]) != nullptr) {
                if (running)
                    components[i]->stop();
                delete components[i];
                components.erase(i);
            }
        }
        return *derived;
    }
}
