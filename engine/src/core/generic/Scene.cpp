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

namespace engine {

	Scene::Scene()
	{
		camera = std::make_shared<Camera>();

		Registry reg;
		reg.Add(SpriteRenderer(glm::vec4(1.0f), Geometry::NONE));
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

	void Scene::AddEntityToScene(Entity* entity)
	{
		if (isRunning) {
			entities.push_back(entity);
			entity->Start();
		}
		else {
			entities.push_back(entity);
		}
		entity->SetScene(this);
	}


	Shr<Camera> Scene::GetCamera() {
		// return the current scene camera, useful for scene testing
		return this->camera;
	}

	glm::vec4 Scene::GetBackcolor() {
		return glm::vec4(this->backcolor, 1.0f);
	}

	void Scene::AddLayer(Layer* layer)
	{
		layer->SetScene(this);
		Application::AddLayer(layer);
	}

	void Scene::AddOverlay(Layer* layer)
	{
		layer->SetScene(this);
		Application::AddOverlay(layer);
	}

	void Scene::RemoveLayer(Layer* layer) const
	{
		Application::RemoveLayer(layer);
	}

	void Scene::RemoveOverlay(Layer* layer) const
	{
		Application::RemoveOverlay(layer);
	}
}
