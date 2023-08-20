#pragma once
#include "Engine.h"
#include "utility.h"

#include "Scene.h"

namespace Paper {

    class Layer;
    class Scene;

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity entity, Scene* scene);
        Entity(entt::entity entity, std::string name, Scene* scene);
        Entity(entt::entity entity, const PaperID& id, std::string name, Scene* scene);

        ~Entity() = default;

        Entity(const Entity& entity) = default;

        template<typename T>
        T& GetComponent();

    	template<typename T>
        bool HasComponent() const;

    	template<typename T, typename... ARGS>
        T& AddComponent(ARGS&... args);

    	template<typename T>
        bool RemoveComponent();

        operator bool() const { return entity != entt::null; }
        operator entt::entity() const { return entity; }
        operator uint32_t() const { return (uint32_t)entity; }
        bool operator==(const Entity& entity) const { return this->entity == entity.entity; }

        Entity* AddTag(std::string tag);
        Entity* AddTag(std::initializer_list<std::string> tags);
        bool RemoveTag(std::string tag);
        bool HasTag(std::string tag);

    	PaperID& GetPaperID();
        Scene* GetScene() const { return scene; }

        std::string GetName();
        void SetName(const std::string& name);

    private:
        Scene* scene = nullptr;

        entt::entity entity = entt::null;
    };

    template <typename T>
    T& Entity::GetComponent()
    {
        CORE_ASSERT(HasComponent<T>(), "Entity does not have this Component");
        return scene->Registry().get<T>(entity);
    }

    template <typename T>
    bool Entity::HasComponent() const
    {
        return scene->Registry().all_of<T>(entity);
    }

    template <typename T, typename ... ARGS>
    T& Entity::AddComponent(ARGS&... args)
    {
        CORE_ASSERT(!HasComponent<T>(), "Entity already has this Component");
        return scene->Registry().emplace<T>(entity, std::forward<ARGS>(args)...);
    }

    template <typename T>
    bool Entity::RemoveComponent()
    {
        CORE_ASSERT(HasComponent<T>(), "Entity does not have this Component");
        return scene->Registry().remove<T>(entity);
    }
}
