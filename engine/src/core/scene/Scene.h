#pragma once
#include "Engine.h"
#include "utility.h"

#include "utils/PaperID.h"

#include "camera/EditorCamera.h"

class b2Body;
class b2World;

namespace Paper {

    class Entity;

    struct SceneConfig
    {
        PaperID uuid;
        std::string name = "NewScene";

        std::filesystem::path path;

        bool show2DColliders = true;
        bool showCameraIcons = true;
    };

    class Scene {
    public:

        Scene() = default;
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

        b2Body* GetB2Body(PaperID entityID) const;

        PaperID GetPaperID() const { return config.uuid; }
        std::string GetName() const { return config.name; }

        std::filesystem::path GetPath() { return config.path; }
        void SetPath(const std::filesystem::path& path ) { config.path = path; }

        auto& Registry() { return registry; }
        auto& EntityMap() { return entity_map; }

        SceneConfig& GetConfig() { return config; }
        const SceneConfig& GetConfig() const { return config; }

        void SetConfig(SceneConfig sceneConfig) { config = sceneConfig; }

        //runtime
        bool IsPaused() const { return isPaused; }
        void SetPaused(const bool paused) { isPaused = paused; }

        void StepFrames(const int frames = 1) { framesToStep = frames; }

        static void SetActive(const Shr<Scene>& newActiveScene) { activeScene = newActiveScene; }
        static Shr<Scene> GetActive() { return activeScene; }
    private:
        SceneConfig config;

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
