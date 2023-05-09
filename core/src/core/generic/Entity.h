#pragma once
#include "_Core.h"

#include "utility.h"

#include "Object.h"
#include "generic/Transform.h"
#include "utils/DataPool.h"
#include "event/Event.h"

namespace core {

    class Component;
    class RenderComponent;
    class Layer;

    class Entity : public Object {
    public:
        Entity(std::string name, const Transform& transform = Transform(), ProjectionMode mode = ProjectionMode::PERSPECTIVE);

        ~Entity() override;

        template<typename T>
        T* GetComponent();

        template<typename T>
        bool RemoveComponent();

        bool AddComponent(Component* component);

        void Update();
        void Start();
        void Stop();
        void Imgui(float dt);
        void OnEvent(Event& event);

        void DeleteComponents();

        Entity* AddTag(std::string tag);
        Entity* AddTag(std::initializer_list<std::string> tags);
        bool RemoveTag(std::string tag);
        bool HasTag(std::string tag);

        Layer* GetLayer() const { return this->layer; }
        bool IsRunning() const { return running; }

        ProjectionMode GetProjectionMode() const
        {
            return mode;
        }

        bool onlyLayerReceive = true;
        

    private:
        std::vector<Component*> components;
        RenderComponent* renderComponent;
        Layer* layer = nullptr;
        bool deleted = false;

        std::initializer_list<std::string> tags;
        ProjectionMode mode;

        bool running = false;


        std::vector<std::string> tagList;

        void StopComponentIndex(int32_t index);
        void DeleteComponentIndex(int32_t index);

        friend class Layer;
        void SetLayer(Layer* layer);

        friend class Scene;
        RenderComponent* GetRenderComponent() const;
    };

    template <typename T>
    T* Entity::GetComponent()
    {
        if (dynamic_cast<T*>(renderComponent) != nullptr)
        {
            return dynamic_cast<T*>(renderComponent);
        }
        for (Component* com : components)
        {
            T* derived = dynamic_cast<T*>(com);
            if (derived != nullptr) return derived;
        }
        return nullptr;
    }

    template <typename T>
    bool Entity::RemoveComponent() {
        if (dynamic_cast<T*>(renderComponent) != nullptr)
        {
            if (running)
                StopComponentIndex(-1);
            DeleteComponentIndex(-1);
            renderComponent = nullptr;
            return true;
        }
        for (int i = 0; i < components.size(); i++)
        {
            if (dynamic_cast<T*>(components[i]) != nullptr) {
                if (running)
                    StopComponentIndex(i);
                DeleteComponentIndex(i);
                components.erase(components.begin() + i);
                return true;
            }
        }
        
        return false;
    }
}
