#include "Engine.h"

#include "generic/Application.h"
#include "generic/Scene.h"

#include "Component.h"
#include "generic/Camera.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"

#include "component/TransformComponent.h"
#include "component/SpriteComponent.h"


namespace engine {

	Scene::Scene()
	{
		camera = std::make_shared<Camera>();
	}

	Scene::~Scene()
	{

	}

	Shr<Camera> Scene::GetCamera()
	{
		return camera;
	}

	void Scene::Update()
	{
		OnUpdate();
		//
		////update entities
		//for (const auto& layer : Application::GetLayerStack())
		//{
		//	if (!layer->IsAttached()) continue;
		//	for (const auto& entity : layer->GetEntitys())
		//	{
		//		if (!entity->IsRunning()) continue;
		//		entity->Update();
		//
		//		if (entity->GetRenderComponent() == nullptr) continue;
		//		entity->GetRenderComponent()->OnUpdate();
		//	}
		//}
	}

	void Scene::Start()
	{
		isRunning = true;
		OnStart();
	}

	void Scene::Stop()
	{
		isRunning = false;
		OnStop();
	}

	void Scene::Render()
	{
		for (const auto sprite_entities = registry.group<TransformComponent>(entt::get<SpriteComponent>); auto & entity : sprite_entities)
		{
			auto [transform, sprite] = sprite_entities.get<TransformComponent, SpriteComponent>(entity);


			EdgeRenderData data;
			data.color = sprite.color;
			data.texture = sprite.texture;
			data.tilingFactor = sprite.tiling_factor;
			data.texCoords = sprite.tex_coords;
			data.mode = sprite.projection_mode;
			data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;

			if (sprite.geometry == Geometry::RECTANGLE)
				Renderer2D::DrawRectangle(data);
			else if (sprite.geometry == Geometry::TRIANGLE)
				Renderer2D::DrawTriangle(data);
		}

		for (const auto circle_entities = registry.group<TransformComponent>(entt::get<SpriteComponent>); auto & entity : sprite_entities)
		{
			auto [transform, sprite] = sprite_entities.get<TransformComponent, SpriteComponent>(entity);


			EdgeRenderData data;
			data.color = sprite.color;
			data.texture = sprite.texture;
			data.tilingFactor = sprite.tiling_factor;
			data.texCoords = sprite.tex_coords;
			data.mode = sprite.projection_mode;
			data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;

			if (sprite.geometry == Geometry::RECTANGLE)
				Renderer2D::DrawRectangle(data);
			else if (sprite.geometry == Geometry::TRIANGLE)
				Renderer2D::DrawTriangle(data);
		}

		for (const auto sprite_entities = registry.group<TransformComponent>(entt::get<SpriteComponent>); auto & entity : sprite_entities)
		{
			auto [transform, sprite] = sprite_entities.get<TransformComponent, SpriteComponent>(entity);


			EdgeRenderData data;
			data.color = sprite.color;
			data.texture = sprite.texture;
			data.tilingFactor = sprite.tiling_factor;
			data.texCoords = sprite.tex_coords;
			data.mode = sprite.projection_mode;
			data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;

			if (sprite.geometry == Geometry::RECTANGLE)
				Renderer2D::DrawRectangle(data);
			else if (sprite.geometry == Geometry::TRIANGLE)
				Renderer2D::DrawTriangle(data);
		}
		////render anything inside the scene
		//RenderCommand::ClearStats();
		//RenderCommand::Clear();
		//
		//RenderCommand::UploadCamera(camera);
		//
		//Renderer3D::BeginRender();
		//for (Entity* entity : entities)
		//{
		//	if (!entity->GetRenderComponent()) continue;
		//	entity->GetRenderComponent()->OnRender();
		//}
		//Renderer3D::EndRender();
		//
		//Renderer2D::BeginRender();
		//
		//for (const auto& layer : Application::GetLayerStack())
		//{
		//	if (!layer->IsAttached()) continue;
		//
		//	for (const auto& entity : layer->GetEntitys())
		//	{
		//		if (!entity->IsRunning()) continue;
		//		if (entity->GetRenderComponent() == nullptr) continue;
		//
		//		entity->GetRenderComponent()->OnRender();
		//	}
		//	Renderer2D::NextBatch(ALL);
		//	layer->RenderUI();
		//}
		//
		//Renderer2D::EndRender();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(const UUID& id, const std::string& name)
	{
		Entity entity(registry.create(), id, name, this);
		entityMap[id] = entity;

		LOG_DEBUG(registry.storage<entt::entity>().in_use());

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity)
	{
		if (!entityMap.contains(entity.GetUUID())) return false;

		entityMap.erase(entity.GetUUID());
		registry.destroy(entity);
		return true;
	}

	Entity Scene::GetEntity(const UUID& id)
	{
		CORE_ASSERT(entityMap.contains(id), "Entity does not exists");
		
		return {entityMap.at(id), this};
	}

}
