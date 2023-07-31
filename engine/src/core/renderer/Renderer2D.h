#pragma once
#include "utility.h"

#include "renderer/Texture.h"
#include "utils/DataPool.h"
#include "camera/EditorCamera.h"
#include "camera/EntityCamera.h"

#include "renderer/Font.h"

#define DEFAULT_COLOR glm::vec4(0.925f, 0.329f, 0.956, 1.0f)

namespace Paper {

    struct EdgeRenderData
    {
        glm::mat4 transform = glm::mat4(1.0f);
        glm::vec4 color = DEFAULT_COLOR;

        Shr<Texture> texture = nullptr;
        std::array<glm::vec2, 4> texCoords = { { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } } };
        float tilingFactor = 1.0f;

        entity_id enity_id = 0;
        entity_id uiID = 0;
        bool coreIDToAlphaPixels = false;
    };

    struct CircleRenderData
    {
        glm::mat4 transform = glm::mat4(1.0f);
        glm::vec4 color = DEFAULT_COLOR;

        float thickness = 1.0f;
        float fade = 0.005f;

        Shr<Texture> texture = nullptr;
        std::array<glm::vec2, 4> texCoords = { { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } } };
        float tilingFactor = 1.0f;

        entity_id enity_id = 0;
        entity_id uiID = 0;
        bool coreIDToAlphaPixels = false;
    };

    struct LineRenderData
    {
        glm::mat4 transform = glm::mat4(1);

        glm::vec3 point0 = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 point1 = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec4 color = DEFAULT_COLOR;

        float thickness = 1.0f;

        entity_id enity_id = 0;
        entity_id uiID = 0;
    };

    struct TextRenderData
    {
        glm::mat4 transform = glm::mat4(1.0f);
        glm::vec4 color = DEFAULT_COLOR;

        std::string text = "";
        Shr<Font> font = DataPool::GetDefaultFont();

        entity_id enity_id = 0;
        entity_id uiID = 0;
        bool coreIDToAlphaPixels = false;
    };

    enum RenderTarget2D
    {
        ALL, RECTANGLE, TRIANGLE, CIRCLE, LINE, TEXT
    };

    struct RenderData2D;

    class Renderer2D {
    public:
        static void Init();
        static void Shutdown();

        static void BeginRender(const Shr<EditorCamera>& camera);
        static void BeginRender(const EntityCamera& camera, glm::mat4 transform);
        static void EndRender();

        static void Render(RenderTarget2D target);

        static void DrawRectangle(const EdgeRenderData& renderData);

        static void DrawTriangle(const EdgeRenderData& renderData);

        static void DrawLine(const LineRenderData& renderData);
        static void DrawLineLegacy(const LineRenderData& renderData);

        static void DrawCircle(const CircleRenderData& renderData);

        static void DrawString(const TextRenderData& renderData);

    	static void NextBatch(RenderTarget2D target);

        static void DrawLineRect(const glm::mat4& transform, const glm::vec4& color, int entityID);


    private:
        static void StartBatch(RenderTarget2D target);
    };

}
