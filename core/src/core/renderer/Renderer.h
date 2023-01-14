#pragma once
#include "utility.h"

#include "renderer/RenderBatch.h"
#include "renderer/FrameBuffer.h"
#include "generic/GameObject.h"
#include "component/SpriteRenderer.h"

namespace core {
    
    class CORE_API Renderer {
    private:
        const int MAX_BATCH_SIZE = 1000;
        std::vector<RenderBatch*> batches;

        FrameBufferProperties properties;
        FrameBuffer* frame_buffer = nullptr;
    public:
        Renderer();
        ~Renderer();
        void add(GameObject* gameObject);
        void add(SpriteRenderer* spriteRenderer);
        // this function has to be called on every frame (update)
        void render(float dt);
        void updateGameObjects(float deltaTime, std::vector<GameObject*>& gameObjects);

        FrameBuffer& GetFrameBuffer() const { return *frame_buffer; }
    };

}