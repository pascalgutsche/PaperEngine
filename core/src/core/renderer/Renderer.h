#pragma once
#include "utility.h"

#include "renderer/RenderBatch.h"
#include "renderer/FrameBuffer.h"
#include "generic/GameObject.h"
#include "component/SpriteRenderer.h"
#include "layer/LayerStack.h"



namespace core {

    

    class Renderer {
    private:
        static Renderer* instance;

        const int MAX_BATCH_SIZE = 1000;
        std::vector<RenderBatch*> batches;
        std::vector<GameObject*> game_objects_in_use;
        FrameBufferProperties properties;
        FrameBuffer* frame_buffer = nullptr;

        inline static int sprites_count = 0;
        inline static int vertex_count = 0;
        inline static std::vector<Shr<Texture>> texturesInUse;

        inline static std::vector<uint32_t> idInUse;
        inline static uint32_t leastAvailableId = 1;

    public:
        Renderer();
        ~Renderer();
        void add(RenderData* renderData);
        void remove(RenderData* renderData);
        // this function has to be called on every frame (update)
        void render(float dt);
        void updateGameObjects(float dt);

        FrameBuffer& GetFrameBuffer() const { return *frame_buffer; }

        static uint32_t RequestID();
        static void RemoveID(uint32_t id);

    	inline static int GetVerticesCount() { return vertex_count; }
        inline static int GetSpriteCount() { return sprites_count; }
        inline static std::vector<Shr<Texture>> GetTexturesInUse() { return texturesInUse; }
        inline static int GetBatchCount() { return instance->batches.size(); }
    };

}