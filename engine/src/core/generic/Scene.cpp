#include "Engine.h"

#include "generic/Application.h"
#include "generic/Scene.h"

#include "EntityCamera.h"
#include "component/CameraComponent.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"


#include "component/TransformComponent.h"
#include "component/SpriteComponent.h"
#include "component/LineComponent.h"
#include "component/TextComponent.h"
#include "component/ScriptComponent.h"
#include "scripting/ScriptEngine.h"


namespace Paper {

	Scene::Scene()
		: uuid(UUID()), name("[Scene]"), is_dirty(true) { }

	Scene::Scene(const UUID& uuid)
		: uuid(uuid), name("[Scene]"), is_dirty(true) { }

	Scene::Scene(const std::string& name)
		: uuid(UUID()), name(name), is_dirty(true) { }

	Scene::Scene(const UUID& uuid, const std::string& name)
		: uuid(uuid), name(name), is_dirty(true) { }

	Scene::~Scene()
	{
		registry.clear();
	}

	void Scene::OnRuntimeStart()
	{

		//Scripting
		{
			ScriptEngine::OnRuntimeStart(this);
			auto view = registry.view<ScriptComponent>();
			for (auto [e, script] : view.each()) {
				Entity entity(e, this);
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnRuntimeUpdate(float dt)
	{
		//Update Scripts
		if (sceneActive)
		{
	
		}
	}

	void Scene::OnEditorUpdate()
	{
		
	}

	void Scene::RuntimeRender()
	{
		EntityCamera* entityCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			auto view = registry.view<CameraComponent, TransformComponent>();
			for (auto [entity, camera, transform] : view.each()) {
				if (camera.primary)
				{
					entityCamera = &camera.camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		//Update rotation in transform component
		{
			auto view = registry.view<TransformComponent>();
			for (auto [entity, transform] : view.each()) {
				transform.UpdateRotation();
			}
		}


		Renderer2D::BeginRender()
		//Render scene
		{
			auto view = registry.view<TransformComponent, SpriteComponent>();
			for (auto [entity, transform, sprite] : view.each()) {
				if (sprite.geometry == Geometry::CIRCLE)
				{
					CircleRenderData data;
					data.transform = transform.GetTransform();
					data.color = sprite.color;
					data.texture = sprite.texture;
					data.tilingFactor = sprite.tiling_factor;
					data.texCoords = sprite.tex_coords;
					data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;
					data.enity_id = (entity_id)entity;

					data.thickness = sprite.thickness;
					data.fade = sprite.fade;

					Renderer2D::DrawCircle(data);
				}
				else
				{
					EdgeRenderData data;
					data.transform = transform.GetTransform();
					data.color = sprite.color;
					data.texture = sprite.texture;
					data.tilingFactor = sprite.tiling_factor;
					data.texCoords = sprite.tex_coords;
					data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;
					data.enity_id = (entity_id)entity;

					if (sprite.geometry == Geometry::RECTANGLE)
						Renderer2D::DrawRectangle(data);
					else if (sprite.geometry == Geometry::TRIANGLE)
						Renderer2D::DrawTriangle(data);
				}
			}
		}

		{
			auto view = registry.view<TransformComponent, LineComponent>();
			for (auto [entity, transform, line] : view.each())
			{
				LineRenderData data;
				data.transform = transform.GetTransform();
				data.color = line.color;
				data.thickness = line.thickness;
				data.enity_id = (entity_id)entity;

				Renderer2D::DrawLine(data);
			}
		}

		{
			auto view = registry.view<TransformComponent, TextComponent>();
			for (auto [entity, transform, text] : view.each()) {

				TextRenderData data;
				data.transform = transform.GetTransform();
				data.color = text.color;
				data.text = text.text;
				data.font = text.font;
				data.coreIDToAlphaPixels = text.register_alpha_pixels_to_event;
				data.enity_id = (entity_id)entity;

				Renderer2D::DrawString(data);
			}
		}
	}

	void Scene::EditorRender(const Shr<EditorCamera>& camera)
	{
		//Update rotation in transform component
		{
			auto view = registry.view<TransformComponent>();
			for (auto [entity, transform] : view.each()) {
				transform.UpdateRotation();
			}
		}

		//Render scene
		{
			auto view = registry.view<TransformComponent, SpriteComponent>();
			for (auto [entity, transform, sprite] : view.each()) {
				if (sprite.geometry == Geometry::CIRCLE)
				{
					CircleRenderData data;
					data.transform = transform.GetTransform();
					data.color = sprite.color;
					data.texture = sprite.texture;
					data.tilingFactor = sprite.tiling_factor;
					data.texCoords = sprite.tex_coords;
					data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;
					data.enity_id = (entity_id)entity;

					data.thickness = sprite.thickness;
					data.fade = sprite.fade;

					Renderer2D::DrawCircle(data);
				}
				else
				{
					EdgeRenderData data;
					data.transform = transform.GetTransform();
					data.color = sprite.color;
					data.texture = sprite.texture;
					data.tilingFactor = sprite.tiling_factor;
					data.texCoords = sprite.tex_coords;
					data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;
					data.enity_id = (entity_id)entity;

					if (sprite.geometry == Geometry::RECTANGLE)
						Renderer2D::DrawRectangle(data);
					else if (sprite.geometry == Geometry::TRIANGLE)
						Renderer2D::DrawTriangle(data);
				}
			}
		}

		{
			auto view = registry.view<TransformComponent, LineComponent>();
			for (auto [entity, transform, line] : view.each()) 
			{
				LineRenderData data;
				data.transform = transform.GetTransform();
				data.color = line.color;
				data.thickness = line.thickness;
				data.enity_id = (entity_id)entity;
				
				Renderer2D::DrawLine(data);
			}
		}

		{
			auto view = registry.view<TransformComponent, TextComponent>();
			for (auto [entity, transform, text] : view.each()) {

				TextRenderData data;
				data.transform = transform.GetTransform();
				data.color = text.color;
				data.text = text.text;
				data.font = text.font;
				data.coreIDToAlphaPixels = text.register_alpha_pixels_to_event;
				data.enity_id = (entity_id)entity;
				
				Renderer2D::DrawString(data);
			}
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		is_dirty = true;
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(const UUID& id, const std::string& name)
	{
		is_dirty = true;
		Entity entity(registry.create(), id, name, this);
		entity_map[id] = entity;

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity)
	{
		if (!entity_map.contains(entity.GetUUID())) return false;

		is_dirty = true;

		entity_map.erase(entity.GetUUID());
		registry.destroy(entity);
		return true;
	}

	Entity Scene::GetEntity(const UUID& id)
	{
		CORE_ASSERT(entity_map.contains(id), "Entity does not exists");
		
		return {entity_map.at(id), this};
	}

	void Scene::SetSceneActive(bool active)
	{
		if (active)
			OnRuntimeStart();
		else
			OnRuntimeStop();
		sceneActive = active;
	}
}
