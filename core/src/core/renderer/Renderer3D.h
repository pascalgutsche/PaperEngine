#pragma once
#include "utility.h"

#include "generic/Camera.h"
#include "generic/Transform.h"
#include "renderer/FrameBuffer.h"
#include "renderer/Texture.h"
#include "utils/DataPool.h"

#include "renderer/Font.h"

#define DEFAULT_COLOR glm::vec4(0.925f, 0.329f, 0.956, 1.0f)

namespace core {

    struct EdgeRenderData3D
    {
        Transform transform = Transform();
        glm::vec4 color = DEFAULT_COLOR;

        Shr<Texture> texture = nullptr;
        glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        float tilingFactor = 1.0f;

        core_id coreID = 0;
    };



    class Renderer3D {
    public:
        static void Init();
        static void Shutdown();

        static void ResizeWindow(uint32_t width, uint32_t height);

        static void BeginRender(Shr<Camera> camera);
        static void EndRender();

        static void Render();

        static void DrawCube(const EdgeRenderData3D& renderData);

        struct Stats
        {
            uint32_t drawCalls = 0;
            uint32_t objectCount = 0;
            uint32_t dataSize = 0;
            uint32_t vertexCount = 0;
            uint32_t elementCount = 0;
        };
        static Stats GetStats();
        static void ClearStats();

        static void NextBatch();
    private:
        static void StartBatch();
    };

}
