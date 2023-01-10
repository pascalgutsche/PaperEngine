#include "_Core.h"

#include "generic/Scene.h"
#include "generic/Camera.h"
#include "renderer/Renderer.h"

#include "imgui/ImGuiLayer.h"

namespace core {

    Scene::Scene() {

    }

    Scene::~Scene() {

    }

    void Scene::initGeneral() {
        camera = new Camera();
        renderer = new Renderer();

        loadResources();
        init();
        start();
    }

    void Scene::init() {

    }

    void Scene::start() {
        // iterate through gameObjects and start all gameObjects in the current scene
        // also add all gameObjects to the current scene renderer
        for (int i = 0; i < gameObjects.size(); i++) {
            gameObjects[i]->start();
            renderer->add(gameObjects[i]);
        }
        isRunning = true;
    }

    void Scene::addGameObjectToScene(GameObject* gameObject) {
        // add desired gameObject to the scene in order to make the current scene render it
        // save the newest gameObject to an used vector place at the end of the used places

        if (isRunning) {
            gameObjects.push_back(gameObject);
            gameObject->start();

            renderer->add(gameObject);
        }
        else {
            gameObjects.push_back(gameObject);
        }
    }

    void Scene::disable() {
        delete camera;
        delete renderer;
    }

    void Scene::update(float deltaTime) {
        // fps are also useful
        std::cout << "[Scene] FPS: " << 1 / deltaTime << std::endl;
    }

    Camera* Scene::getCamera() {
        // return the current scene camera, useful for scene testing
        return this->camera;
    }

    void Scene::sceneImgui(float deltaTime) {
        if (activeGameObject != nullptr) {
            ImGui::Begin(std::string("Inspector - " + activeGameObject->getName()).c_str());
            activeGameObject->imgui(deltaTime);
            ImGui::End();
        }
        imgui(deltaTime);
    }

    void Scene::imgui(float deltaTime) {

    }

    glm::vec4 Scene::getBackcolor() {
        return this->backcolor;
    }

    std::vector<GameObject*> Scene::getGameObjects() {
        return this->gameObjects;
    }

    void Scene::loadResources() {

    }

}