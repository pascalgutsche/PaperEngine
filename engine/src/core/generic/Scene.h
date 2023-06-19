#pragma once
#include "Engine.h"
#include "utility.h"

namespace engine {

    class Entity;
    class Camera;
    class Layer;
    class Event;

    class Scene {
        friend class Application;
    public:

        Scene();
        virtual ~Scene() = default;

        Shr<Camera> GetCamera();
        glm::vec4 GetBackcolor();

        void Start();


        void Stop();

        void Render();

        void AddEntityToScene(Entity* entity);

        void AddLayer(Layer* layer);
        void AddOverlay(Layer* layer);
        void RemoveLayer(Layer* layer) const;
        void RemoveOverlay(Layer* layer) const;

        std::vector<Entity*>& GetEntitys() { return entities; }

        entt::registry& Registry() { return registry; }

    protected:
        Shr<Camera> camera = nullptr;
        glm::vec3 backcolor = glm::vec3(1.0f, 0.1f, 0.6f);

        virtual void OnStart() = 0;
        virtual void OnStop() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnEvent(Event& e) = 0;
        virtual void Imgui(float dt) {}

    private:
        std::vector<Entity*> entities;
        bool isRunning = false;

        void Update();

        entt::registry registry;
    };

}