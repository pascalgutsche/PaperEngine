#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Shader.h"
#include "renderer/Texture.h"
#include "utils/DataPool.h"

#include "renderer/VertexArray.h"
#include "renderer/Buffer.h"

namespace core {

    class RenderBatch;

    struct RenderData
    {
        std::vector<float> vertices;
        std::vector<unsigned int> ebo;
        std::vector<Shr<Texture>> textures;

        int zIndex;
        bool dirty;

        ProjectionMode projectionMode;

        RenderData()
        {
            vertices.resize(0);
            ebo.resize(0);
            textures.resize(0);
            zIndex = 0;
            projectionMode = ProjectionMode::PERSPECTIVE;
            dirty = false;
        }

    private:
        int id = 0;

        std::vector<Shr<Texture>> oldTextures;

        friend class RenderBatch;
    };

    class RenderBatch {
    private:
        static constexpr int POS_SIZE = 2;
        static constexpr int COLOR_SIZE = 4;
        static constexpr int TEX_COORDS_SIZE = 2;
        static constexpr int TEX_ID_SIZE = 1;

        static constexpr int POS_OFFSET = 0;
        static constexpr int COLOR_OFFSET = sizeof(float) * (POS_SIZE);
        static constexpr int TEX_COORDS_OFFSET = sizeof(float) * (POS_SIZE + COLOR_SIZE);
        static constexpr int TEX_ID_OFFSET = sizeof(float) * (POS_SIZE + COLOR_SIZE + TEX_COORDS_SIZE);
        static constexpr int VERTEX_SIZE = POS_SIZE + COLOR_SIZE + TEX_COORDS_SIZE + TEX_ID_SIZE;
        static constexpr int VERTEX_SIZE_BYTES = sizeof(float) * VERTEX_SIZE;

        static constexpr int MAX_VERTEX_COUNT = 3000;
        static constexpr int MAX_ELEMENT_COUNT = 5000;

        std::vector<int> texSlots = { 0, 1, 2, 3, 4, 5, 6, 7 };
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<RenderData*> dataBlocks;


        int zIndex;
        int oldVertexSize = -1;
        int oldElementSize = -1;

        Shr<Shader> shader;

        int structCount = 0;

        inline static int draw_calls = 0;
        inline static bool enable_polygon = false;
        inline static int polygonMode = 6914;

        ProjectionMode projectionMode;

        Shr<VertexArray> vertexArray;
        Shr<VertexBuffer> vertexBuffer;
        Shr<ElementBuffer> elementBuffer;

    public:

        RenderBatch(int zIndex, ProjectionMode mode);
        ~RenderBatch();
        void start();
        int render();

        bool HasTextureSpace(std::vector<Shr<Texture>> texs);

        void addVertexProperties(RenderData* renderdata);
        void updateVertexProperties(RenderData* renderdata);
        void removeVertexProperties(RenderData* renderdata);


        inline static void SetPolygonMode(int mode)
        {
            polygonMode = mode;
        }

        inline ProjectionMode GetProjectionMode()
        {
            return projectionMode;
        }

        inline static int GetDrawCalls() { 
            return draw_calls; 
        }

        inline int GetZIndex() const {
            return zIndex;
        }

        inline int GetTextureCount() const {
            return textures.size();
        }

        inline int GetVertexCount() const
        {
            return 0;
        }

        inline std::vector<Shr<Texture>> GetTexturesInUse()
        {
            return this->textures;
        }

        static inline int GetVertexSize()
        {
            return VERTEX_SIZE;
        }
    };

}