#pragma once
#include "Engine.h"
#include "utility.h"

#include "utils/EntityID.h"

#include "camera/EditorCamera.h"

namespace Paper {

    class Entity;

    class Scene {
        friend class Application;
        friend class YAMLSerializer;
    public:

        Scene();
        Scene(const EntityID& uuid);
        Scene(const std::string& name);
        Scene(const EntityID& uuid, const std::string& name);
        ~Scene();

        Shr<Scene> Copy();

        void OnViewportResize(uint32_t width, uint32_t height);

        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnRuntimeUpdate();

    	void OnSimulationStart();
        void OnSimulationStop();
        void OnSimulationUpdate(const Shr<EditorCamera>& camera);

        void OnEditorUpdate(const Shr<EditorCamera>& camera);

        void EditorRender(const Shr<EditorCamera>& camera);
        void Render();

        Entity CreateEntity(const std::string& name);
        Entity CreateEntity(const EntityID& id, const std::string& name);
        bool DestroyEntity(Entity entity);

        Entity GetEntity(const EntityID& id);
        Entity GetEntityByName(const std::string& name);

        EntityID GetEntityID() const { return uuid; }
        std::string GetName() const { return name; }

        std::filesystem::path GetPath() { return path; }
        void SetPath(const std::filesystem::path& path ) { this->path = path; }

        auto& Registry() { return registry; }
        auto& EntityMap() { return entity_map; }

        bool IsDirty() const { return is_dirty; }
        void SetClean() { is_dirty = false; }

        //runtime
        bool IsPaused() const { return isPaused; }
        void SetPaused(const bool paused) { isPaused = paused; }

        void StepFrames(const int frames = 1) { framesToStep = frames; }
    private:
        EntityID uuid;
        std::string name;

        bool is_dirty = false;

        std::filesystem::path path;

        entt::registry registry;
        std::unordered_map<EntityID, entt::entity> entity_map;

        //runtime
        bool isPaused = false;
        int framesToStep = 0;
    };

    

}