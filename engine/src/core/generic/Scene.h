#pragma once
#include "Engine.h"
#include "utility.h"

#include "utils/UUID.h"

namespace engine {

    class Entity;
    class Camera;
    class Layer;
    class Event;

    class Scene {
        friend class Application;
    public:

        Scene();
        virtual ~Scene();

        Shr<Camera> GetCamera();

        void Start();


        void Stop();

        void Render();

        Entity CreateEntity(const std::string& name);
        Entity CreateEntity(const UUID& id, const std::string& name);
        bool DestroyEntity(Entity entity);

        Entity GetEntity(const UUID& id);

        auto& Registry() { return registry; }
        auto& EntityMap() { return entityMap; }

        entt::registry registry;
    protected:
        Shr<Camera> camera = nullptr;

        virtual void OnStart() = 0;
        virtual void OnStop() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnEvent(Event& e) = 0;
        virtual void Imgui(float dt) {}

    private:
        bool isRunning = false;

        void Update();


        std::unordered_map<UUID, entt::entity> entityMap;
    };

}