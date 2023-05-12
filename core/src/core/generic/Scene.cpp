#include "_Core.h"

#include "generic/Application.h"
#include "generic/Scene.h"

#include "Component.h"
#include "generic/Camera.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"
#include "renderer/RenderCommand.h"


namespace core {

    Scene::Scene()
    {
        camera = std::make_shared<Camera>();
    }

    void Scene::Update()
    {
        OnUpdate();

        //render anything inside the scene
        Renderer2D::ClearStats();
        Renderer3D::ClearStats();
        RenderCommand::Clear();
        RenderCommand::PrepareFramebuffer();

        RenderCommand::UploadCamera(camera);

        Renderer3D::BeginRender();
        for (Entity* entity : entities)
        {
            if (!entity->GetRenderComponent()) continue;
            entity->GetRenderComponent()->OnUpdate();
        }
        Renderer3D::EndRender();

        Renderer2D::BeginRender();

        for (int i = 0; i < Application::GetLayerStack().GetSize(); i++)
        {
            Layer* layer = Application::GetLayerStack()[i];
            if (!layer->IsAttached()) continue;

            for (int j = 0; j < layer->GetEntitys().size(); j++)
            {
                Entity* gameObject = layer->GetEntitys()[j];
                if (!gameObject->IsRunning()) continue;

                //update all non rendercomponents
                gameObject->Update();

                //update rendercomponent -> only one component that renders
                gameObject->GetRenderComponent()->OnUpdate();
            }
            Renderer2D::NextBatch(ALL);
            layer->RenderUI();
        }

        Renderer2D::EndRender();


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