#pragma once
#include "Engine.h"
#include "utility.h"

#include "utils/PaperID.h"

#include "camera/EditorCamera.h"

class b2Body;
class b2World;

namespace Paper {

    class Entity;

    class Scene {
    public:

        Scene();
        Scene(const PaperID& uuid);
        Scene(const std::string& name);
        Scene(const PaperID& uuid, const std::string& name);
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

        void OnPhysics2DStart();
        void OnPhysics2DStop();

        Entity CreateEntity(const std::string& name);
        Entity CreateEntity(const PaperID& id, const std::string& name);

        Entity DuplicateEntity(Entity entity);

    	bool DestroyEntity(Entity entity);

        Entity GetEntity(const PaperID& id);
        Entity GetEntityByName(const std::string& name);

        PaperID GetPaperID() const { return uuid; }
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

        static void SetActive(const Shr<Scene>& newActiveScene) { activeScene = newActiveScene; }
        static Shr<Scene> GetActive() { return activeScene; }
    private:
        PaperID uuid;
        std::string name;

        bool is_dirty = false;

        std::filesystem::path path;

        entt::registry registry;
        std::unordered_map<PaperID, entt::entity> entity_map;
        std::unordered_map<PaperID, b2Body*> b2BodyMap;

        //runtime
        bool isPaused = false;
        int framesToStep = 0;

        b2World* physicsWorld = nullptr;

        //static
        inline static Shr<Scene> activeScene = nullptr;

        friend class Application;
        friend class SceneSerializer;
    };

    

}
