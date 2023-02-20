#pragma once
#include "utility.h"

#include "renderer/FrameBuffer.h"
#include "generic/Camera.h"

namespace core {

    class Renderer {
    public:
        static void Init();
        static void Shutdown();

        static void BeginRender(const Camera& camera);
        static void EndRender();

        static void Render();

        static void DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec4 color, core_id coreID = -1);
        static void DrawRectangle(glm::mat4 transform, glm::vec4 color, core_id coreID = -1);
        //static void DrawTriangle();
        //static void DrawCircle();
        //static void DrawLine();
        //static void DrawGlyphs();

    private:

        static void StartBatch();
        static void NextBatch();

        
/*
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
        */
    };

}