#pragma once
#include "Engine.h"
#include "utility.h"

#include "utils/UUID.h"

#include "camera/EditorCamera.h"

namespace Paper {

    class Entity;

    class Scene {
        friend class Application;
        friend class YAMLSerializer;
    public:

        Scene();
        Scene(const UUID& uuid);
        Scene(const std::string& name);
        Scene(const UUID& uuid, const std::string& name);
        ~Scene();

        Shr<Scene> Copy();

        void OnViewportResize(uint32_t width, uint32_t height);

        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnRuntimeUpdate();

        void OnEditorUpdate();

        void RuntimeRender();
        void EditorRender(const Shr<EditorCamera>& camera);

        Entity CreateEntity(const std::string& name);
        Entity CreateEntity(const UUID& id, const std::string& name);
        bool DestroyEntity(Entity entity);

        Entity GetEntity(const UUID& id);
        UUID GetUUID() const { return uuid; }
        std::string GetName() const { return name; }

        std::filesystem::path GetPath() { return path; }
        void SetPath(const std::filesystem::path& path ) { this->path = path; }

        auto& Registry() { return registry; }
        auto& EntityMap() { return entity_map; }

        bool IsDirty() const { return is_dirty; }
        void SetClean() { is_dirty = false; }

    private:
        UUID uuid;
        std::string name;

        bool is_dirty = false;

        std::filesystem::path path;

        entt::registry registry;
        std::unordered_map<UUID, entt::entity> entity_map;
    };

    

}