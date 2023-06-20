#include "Engine.h"

#include "generic/Application.h"
#include "generic/Scene.h"

#include "Component.h"
#include "generic/Camera.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"
#include "renderer/RenderCommand.h"

#include "Registry.h"

#include "uuid.h"
#include "component/2D/SpriteRenderer.h"
#include "utils/Core.h"

namespace engine {

	Scene::Scene()
	{
		camera = std::make_shared<Camera>();
	}

	void Scene::Update()
	{
		OnUpdate();

		//update entities
		for (const auto& layer : Application::GetLayerStack())
		{
			if (!layer->IsAttached()) continue;
			for (const auto& entity : layer->GetEntitys())
			{
				if (!entity->IsRunning()) continue;
				entity->Update();

				if (entity->GetRenderComponent() == nullptr) continue;
				entity->GetRenderComponent()->OnUpdate();
			}
		}
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
		//render anything inside the scene
		RenderCommand::ClearStats();
		RenderCommand::Clear();

		RenderCommand::UploadCamera(camera);

		Renderer3D::BeginRender();
		for (Entity* entity : entities)
		{
			if (!entity->GetRenderComponent()) continue;
			entity->GetRenderComponent()->OnRender();
		}
		Renderer3D::EndRender();

		Renderer2D::BeginRender();

		for (const auto& layer : Application::GetLayerStack())
		{
			if (!layer->IsAttached()) continue;

			for (const auto& entity : layer->GetEntitys())
			{
				if (!entity->IsRunning()) continue;
				if (entity->GetRenderComponent() == nullptr) continue;

				entity->GetRenderComponent()->OnRender();
			}
			Renderer2D::NextBatch(ALL);
			layer->RenderUI();
		}

		Renderer2D::EndRender();

		uuids::uuid const id = uuids::uuid_system_generator{}();
		assert(!id.is_nil());
		assert(id.version() == uuids::uuid_version::random_number_based);
		assert(id.variant() == uuids::uuid_variant::rfc);
	}

	Entity& Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(uuid_system_generator{}(), name);
	}

	Entity& Scene::CreateEntity(const uuid& id, const std::string& name)
	{
		Entity entity(id, name, this);
		entityMap[id] = entity;
		return entity;
	}

	bool Scene::DestroyEntity(Entity& entity)
	{
		if (!entityMap.contains(entity.GetUUID())) return false;

		entityMap.erase(entity.GetUUID());
		return true;
	}

	Entity& Scene::GetEntity(const uuid& id) const
	{
		CORE_ASSERT(entityMap.contains(id), "Entity does not exists");
		return entityMap.at(id);
	}

	glm::vec4 Scene::GetBackcolor() {
		return glm::vec4(this->backcolor, 1.0f);
	}
}
