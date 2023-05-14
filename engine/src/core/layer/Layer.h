#pragma once
#include "Engine.h"
#include "utility.h"

#include "event/Event.h"
#include "generic/Entity.h"
#include "generic/Scene.h"

namespace engine
{
	class UIObject;
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: name(name) { }

		virtual ~Layer();

		void Attach();
		void Detach();

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void Update(const float dt) = 0;
		virtual void Imgui(const float dt) { };
		virtual void OnEvent(Event& event) = 0;

		void LayerEvent(Event& event);
		bool IsAttached() const { return attached; }

		void SetScene(Scene* scene) { this->scene = scene; }

		template <class T>
		T* GetScene()
		{
			T* tmpScene = dynamic_cast<T>(scene);
			CORE_ASSERT(tmpScene, "this scene doesnt own this layer")
				return tmpScene;
		}

		void AddEntityToLayer(Entity* game_object);

		std::vector<Entity*> GetEntitysByTag(std::initializer_list<std::string> tags);
		std::vector<Entity*> GetEntitysByTag(std::string tag);


		void SetOverlayStatus(bool overlay) { this->overlay = overlay; }

		bool GetOverlayStatus() const { return overlay; }
		std::vector<Entity*>& GetEntitys() { return gameObjects; }
		std::string GetName() { return name; }

		//UI
		friend class Scene;
		void AddUIObject(UIObject* object, ProjectionMode mode);
		void RemoveUIObject();
		void RemoveUIObject(uint32_t index);
		void RemoveUIObject(UIObject* object);

		std::vector<UIObject*>& GetUIObjects();
	protected:
		Scene* scene = nullptr;
		std::string name;
		bool overlay = false;
		bool attached = false;

		std::vector<Entity*> gameObjects;

	private:
		std::vector<UIObject*> uiObjects;

		void RenderUI();
		void RenderObject(UIObject* object);
	};
}