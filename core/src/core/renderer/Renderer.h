#pragma once
#include "utility.h"

#include "renderer/RenderBatch.h"
#include "renderer/FrameBuffer.h"
#include "generic/GameObject.h"

#include "renderer/RenderCommand.h"
#include "generic/Camera.h"

namespace core {

    

    class Renderer {
    public:
        static void Init();
        static void Shutdown();

        static void BeginRender(Shr<Camera>& camera);
        static void EndRender();

        static void SubmitData(Shr<VertexArray>& vertexArray);




    private:
        static Renderer* instance;

        const int MAX_BATCH_SIZE = 1000;
        std::vector<RenderBatch*> batches;
        std::vector<GameObject*> game_objects_in_use;
        FramebufferSpecification properties;
        Shr<Framebuffer> framebuffer = nullptr;

        int mouseHoverID[2] = { -1, -1 };
        int mouseClickedID[2] = { -1, -1 };
        bool pressed = false;

        inline static int sprites_count = 0;
        inline static int vertex_count = 0;

        glm::vec2 viewportSize;
    public:
        Renderer();
        ~Renderer();
        void add(RenderData* renderData);
        void remove(RenderData* renderData);
        // this function has to be called on every frame (update)
        void render(float dt);
        void updateGameObjects(float dt);

        Shr<Framebuffer> GetFrameBuffer() const { return framebuffer; }
    };

}