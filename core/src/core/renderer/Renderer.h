#pragma once

#include "renderer/RenderBatch.h"
#include "generic/GameObject_Component.h"
#include "component/SpriteRenderer.h"

class Renderer {
private:
    const int MAX_BATCH_SIZE = 1000;
    std::vector<RenderBatch*> batches;
public:
    Renderer();
    ~Renderer();
    void add(GameObject* gameObject);
    void add(SpriteRenderer* spriteRenderer);
    // this function has to be called on every frame (update)
    void render();
    void updateGameObjects(float deltaTime, std::vector<GameObject*> &gameObjects);
};
