#pragma once
#include "_Core.h"

class Scene;
#include "generic/Camera.h"
#include "renderer/Renderer.h"
#include "generic/GameObject_Component.h"
#include "renderer/ImGuiLayer.h"

class Scene {
private:
    bool isRunning = false;
protected:
    Camera* camera;
    Renderer* renderer = new Renderer();
    GameObject* activeGameObject = nullptr;
    ImGuiLayer* imGuiLayer;
    glm::vec4 backcolor;
public:
    static std::unordered_map<std::string, Scene*> sceneMap;
    std::vector<GameObject*> gameObjects;
public:
    Scene();
    ~Scene();

    Camera* getCamera();
    ImGuiLayer* getImGuiLayer();
    glm::vec4 getBackcolor();
    std::vector<GameObject*> getGameObjects();

    void addGameObjectToScene(GameObject* gameObject);
    void start();
    void disable();
    void sceneImgui(float deltaTime);
    void initGeneral();

    virtual void imgui(float deltaTime);
    virtual void init();
    virtual void loadResources();
    virtual void update(float deltaTime);
};