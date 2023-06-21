#include "Engine.h"

#include "generic/Application.h"
#include "generic/Scene.h"

#include "Component.h"
#include "generic/Camera.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"


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

	Entity& Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}

	Entity& Scene::CreateEntity(const UUID& id, const std::string& name)
	{
		Entity entity(id, name, this);
		entityMap[id] = entity;
		LOG_DEBUG(registry.storage<entt::entity>().in_use());

		return entity;
	}

	bool Scene::DestroyEntity(Entity& entity)
	{
		if (!entityMap.contains(entity.GetUUID())) return false;

		entityMap.erase(entity.GetUUID());
		return true;
	}

	Entity& Scene::GetEntity(const UUID& id)
	{
		CORE_ASSERT(entityMap.contains(id), "Entity does not exists");
		return entityMap.at(id);
	}

}
