#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Shader.h"
#include "renderer/Texture.h"
#include "component/SpriteRenderer.h"
#include "utils/DataPool.h"

namespace core {

    class RenderBatch;

    struct RenderData
    {
        std::vector<float> vertices;
        std::vector<unsigned int> ebo;
        std::vector<Shr<Texture>> textures;

        int zIndex;
        bool dirty;

        DataPool::DISPLAYMODE displayMode;

        RenderData()
        {
            vertices.resize(0);
            ebo.resize(0);
            textures.resize(0);
            dirty = false;

            vertexSlot = -1;
            elementSlot = -1;
            dataBlockSlot = -1;
            oldVertexSize = -1;
            oldElementSize = -1;
        }

    private:
        int vertexSlot;
        int elementSlot;

        int dataBlockSlot;

        int oldVertexSize;
        int oldElementSize;


        friend class RenderBatch;
    };

    class RenderBatch {
    private:
        static const int POS_SIZE = 2;
        static const int COLOR_SIZE = 4;
        static const int TEX_COORDS_SIZE = 2;
        static const int TEX_ID_SIZE = 1;

        static const int POS_OFFSET = 0;
        static const int COLOR_OFFSET = sizeof(float) * (POS_SIZE);
        static const int TEX_COORDS_OFFSET = sizeof(float) * (POS_SIZE + COLOR_SIZE);
        static const int TEX_ID_OFFSET = sizeof(float) * (POS_SIZE + COLOR_SIZE + TEX_COORDS_SIZE);
        static const int VERTEX_SIZE = POS_SIZE + COLOR_SIZE + TEX_COORDS_SIZE + TEX_ID_SIZE;
        static const int VERTEX_SIZE_BYTES = sizeof(float) * VERTEX_SIZE;

        bool hasRoom_bool = false;

        static constexpr int MAX_VERTEX_BUFFER_COUNT = 20000;

        std::vector<int> texSlots = { 0, 1, 2, 3 , 4, 5, 6, 7 };
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<RenderData*> dataBlocks;

        unsigned int vaoID;
        unsigned int vboID;
        unsigned int eboID;

        int numSprites;
        int maxBatchSize;
        int zIndex;
        int oldVertexSize = -1;
        int oldElementSize = -1;

        bool reloadBufferArrays = false;

        Shr<Shader> shader;
        std::vector<float> vertices;
        std::vector<unsigned int> elements;

        void updateTextures();

        int structCount = 0;


        inline static int draw_calls = 0;
        inline static bool enable_polygon = false;
        inline static int polygonMode = 6914;

        DataPool::DISPLAYMODE displayMode;

    public:

        RenderBatch(int zIndex, DataPool::DISPLAYMODE displaymode);
        ~RenderBatch();
        void start();
        int render();

        bool hasRoom();
        bool hasTextureRoom();
        bool hasTexture(Shr<Texture> texture);

        void addVertexProperties(RenderData* renderdata);
        void updateVertexProperties(RenderData* renderdata);
        void removeVertexProperties(RenderData* renderdata);


        inline static void SetPolygonMode(int mode)
        {
            polygonMode = mode;
        }

        DataPool::DISPLAYMODE const GetDisplayMode()
        {
            return displayMode;
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
            return vertices.size() / VERTEX_SIZE;
        }

        static inline int GetVertexSize()
        {
            return VERTEX_SIZE;
        }
    };

}