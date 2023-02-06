#include "_Core.h"

#include "imgui/ImGuiLayer.h"
#include "generic/Application.h"
#include "generic/Scene.h"
#include "generic/Camera.h"
#include "renderer/Renderer.h"


namespace core {

    Scene::Scene() {

    }

    Scene::~Scene() {

    }

    void Scene::InitGeneral() {
        camera = new Camera();
        renderer = new Renderer();

        loadResources();
        init();
        Start();
    }

    void Scene::Render()
    {
    }

    void Scene::Start() {
        isRunning = true;
    }

    void Scene::Disable() {
        delete camera;
    }

    Camera* Scene::GetCamera() {
        // return the current scene camera, useful for scene testing
        return this->camera;
    }

    glm::vec4 Scene::GetBackcolor() {
        return this->backcolor;
    }
}