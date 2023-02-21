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

        static void DrawTriangle(glm::vec2 position, glm::vec2 size, glm::vec4 color, core_id coreID = -1);
        static void DrawTriangle(glm::mat4 transform, glm::vec4 color, core_id coreID = -1);

    	//static void DrawCircle();
        //static void DrawLine();
        //static void DrawGlyphs();

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

    private:

        static void StartBatch();
        static void NextBatch();

        

		
    };

}