#pragma once
#include "utility.h"

#include "generic/Transform.h"
#include "renderer/Texture.h"

#include "renderer/Font.h"

#define DEFAULT_COLOR glm::vec4(0.925f, 0.329f, 0.956, 1.0f)

namespace ppr {

    struct EdgeRenderData3D
    {
        Transform transform = Transform();
        glm::vec4 color = DEFAULT_COLOR;
        int isLightSource = false;

        Shr<Texture> texture = nullptr;
        glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        float tilingFactor = 1.0f;

        entity_id entity_id = 0;
    };



    class Renderer3D {
    public:
        static void Init();
        static void Shutdown();

        static void ResizeWindow(uint32_t width, uint32_t height);

        static void BeginRender();
        static void EndRender();

        static void Render();

        static void DrawCube(const EdgeRenderData3D& renderData);


        static void NextBatch();
    private:
        static void StartBatch();
    };

}
