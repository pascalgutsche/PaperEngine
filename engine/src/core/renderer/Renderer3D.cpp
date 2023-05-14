#include "Engine.h"

#include "renderer/Renderer3D.h"
#include "renderer/RenderCommand.h"
#include "utils/DataPool.h"
#include "renderer/Shader.h"
#include "generic/Application.h"
#include "imgui/ImGuiLayer.h"

namespace engine {

    struct EdgeVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        int isLightSource;
        glm::vec3 normal;
        glm::vec2 texCoords;
        float tilingFactor;
        int texIndex;
        int coreID;
    };

    struct RenderData3D
    {
        static constexpr uint32_t MAX_VERTICES = 40000;
        static constexpr uint32_t MAX_ELEMENTS = 60000;
        static constexpr uint32_t MAX_TEXTURE_SLOTS = 32;

        Shr<Shader> edgeGeometryShader;

        Shr<VertexArray> cubeVertexArray;
        Shr<VertexBuffer> cubeVertexBuffer;
        
        uint32_t cubeElementCount = 0;
        EdgeVertex* cubeVertexBufferBase = nullptr;
        EdgeVertex* cubeVertexBufferPtr = nullptr;

        std::array<Shr<Texture>, MAX_TEXTURE_SLOTS> cubeTextureSlots;
        uint32_t cubeTextureSlotIndex = 0;

        std::array<glm::vec3, 24> cubePositionData;
        std::array<glm::vec3, 24> cubeNormalData;

        struct Light
        {
            glm::vec3 position;
        };

        Shr<StorageBuffer> storageBuffer;
        
    };

    static RenderData3D data;
    static int texSlots[data.MAX_TEXTURE_SLOTS - 1];

    void Renderer3D::Init()
    {
        BufferLayout edgeGeometryLayout = {
            { GLSLDataType::FLOAT3, "aPos" },
            { GLSLDataType::FLOAT4, "aColor" },
            { GLSLDataType::INT, "aIsLightSource" },
            { GLSLDataType::FLOAT3, "aNormal" },

            { GLSLDataType::FLOAT2, "aTexCoord" },
            { GLSLDataType::FLOAT , "aTilingFactor"},
            { GLSLDataType::INT , "aTexID" },

        	{ GLSLDataType::INT , "aCoreID" },
        };

        data.edgeGeometryShader = DataPool::GetShader("EdgeGeometryShader_3D");
        data.edgeGeometryShader->Compile();

        data.cubeVertexArray = VertexArray::CreateArray();
        data.cubeVertexBuffer = VertexBuffer::CreateBuffer(edgeGeometryLayout, data.MAX_VERTICES * sizeof(EdgeVertex));
        data.cubeVertexArray->SetVertexBuffer(data.cubeVertexBuffer);
        

        uint32_t* cubeElements = new uint32_t[data.MAX_ELEMENTS];

        int offsetCube = 0;
        for (int i = 0; i < data.MAX_ELEMENTS; i += 6)
        {
            // first triangle
            cubeElements[i + 0] = offsetCube + 0;
            cubeElements[i + 1] = offsetCube + 1;
            cubeElements[i + 2] = offsetCube + 2;
            // second triangle               
            cubeElements[i + 3] = offsetCube + 2;
            cubeElements[i + 4] = offsetCube + 3;
            cubeElements[i + 5] = offsetCube + 0;

            offsetCube += 4;
        }

        Shr<ElementBuffer> cubeElementbuffer = ElementBuffer::CreateBuffer(cubeElements, data.MAX_ELEMENTS);
        data.cubeVertexArray->SetElementBuffer(cubeElementbuffer);
        delete[] cubeElements;

        ///////POSITION
        //back
        data.cubePositionData[0]  = glm::vec3(-0.5f, -0.5f, -0.5f);
        data.cubePositionData[1]  = glm::vec3( 0.5f, -0.5f, -0.5f);
        data.cubePositionData[2]  = glm::vec3( 0.5f,  0.5f, -0.5f);
        data.cubePositionData[3]  = glm::vec3(-0.5f,  0.5f, -0.5f);
                                            
        //front                             
        data.cubePositionData[4]  = glm::vec3(-0.5f, -0.5f,  0.5f);
        data.cubePositionData[5]  = glm::vec3( 0.5f, -0.5f,  0.5f);
        data.cubePositionData[6]  = glm::vec3( 0.5f,  0.5f,  0.5f);
        data.cubePositionData[7]  = glm::vec3(-0.5f,  0.5f,  0.5f);
                                            
        //bottom                            
    	data.cubePositionData[8]  = glm::vec3(-0.5f, -0.5f, -0.5f);
        data.cubePositionData[9]  = glm::vec3( 0.5f, -0.5f, -0.5f);
        data.cubePositionData[10] = glm::vec3( 0.5f, -0.5f,  0.5f);
        data.cubePositionData[11] = glm::vec3(-0.5f, -0.5f,  0.5f);
                                            
        //top                               
        data.cubePositionData[12] = glm::vec3(-0.5f,  0.5f, -0.5f);
        data.cubePositionData[13] = glm::vec3( 0.5f,  0.5f, -0.5f);
        data.cubePositionData[14] = glm::vec3( 0.5f,  0.5f,  0.5f);
        data.cubePositionData[15] = glm::vec3(-0.5f,  0.5f,  0.5f);
                                            
        //left                              
        data.cubePositionData[16] = glm::vec3(-0.5f, -0.5f,  0.5f);
        data.cubePositionData[17] = glm::vec3(-0.5f, -0.5f, -0.5f);
        data.cubePositionData[18] = glm::vec3(-0.5f,  0.5f, -0.5f);
        data.cubePositionData[19] = glm::vec3(-0.5f,  0.5f,  0.5f);
                                            
        //right                             
        data.cubePositionData[20] = glm::vec3( 0.5f, -0.5f,  0.5f);
        data.cubePositionData[21] = glm::vec3( 0.5f, -0.5f, -0.5f);
        data.cubePositionData[22] = glm::vec3( 0.5f,  0.5f, -0.5f);
        data.cubePositionData[23] = glm::vec3( 0.5f,  0.5f,  0.5f);

        /////// NORMALS
        //back
        data.cubeNormalData[0]  = glm::vec3( 0.0f, 0.0f, -1.0f);
        data.cubeNormalData[1]  = glm::vec3( 0.0f, 0.0f, -1.0f);
        data.cubeNormalData[2]  = glm::vec3( 0.0f, 0.0f, -1.0f);
        data.cubeNormalData[3]  = glm::vec3( 0.0f, 0.0f, -1.0f);
                                  
        //front                   
        data.cubeNormalData[4]  = glm::vec3( 0.0f, 0.0f,  1.0f);
        data.cubeNormalData[5]  = glm::vec3( 0.0f, 0.0f,  1.0f);
        data.cubeNormalData[6]  = glm::vec3( 0.0f, 0.0f,  1.0f);
        data.cubeNormalData[7]  = glm::vec3( 0.0f, 0.0f,  1.0f);
                                 
        //bottom
        data.cubeNormalData[8]  = glm::vec3( 0.0f,-1.0f, 0.0f);
        data.cubeNormalData[9]  = glm::vec3( 0.0f,-1.0f, 0.0f);
        data.cubeNormalData[10] = glm::vec3( 0.0f,-1.0f, 0.0f);
        data.cubeNormalData[11] = glm::vec3( 0.0f,-1.0f, 0.0f);
                               
        //top                  
        data.cubeNormalData[12] = glm::vec3( 0.0f, 1.0f, 0.0f);
        data.cubeNormalData[13] = glm::vec3( 0.0f, 1.0f, 0.0f);
        data.cubeNormalData[14] = glm::vec3( 0.0f, 1.0f, 0.0f);
        data.cubeNormalData[15] = glm::vec3( 0.0f, 1.0f, 0.0f);
                                  
        //left                    
        data.cubeNormalData[16] = glm::vec3(-1.0f, 0.0f, 0.0f);
        data.cubeNormalData[17] = glm::vec3(-1.0f, 0.0f, 0.0f);
        data.cubeNormalData[18] = glm::vec3(-1.0f, 0.0f, 0.0f);
        data.cubeNormalData[19] = glm::vec3(-1.0f, 0.0f, 0.0f);
                                   
        //right                    
        data.cubeNormalData[20] = glm::vec3( 1.0f, 0.0f, 0.0f);
        data.cubeNormalData[21] = glm::vec3( 1.0f, 0.0f, 0.0f);
        data.cubeNormalData[22] = glm::vec3( 1.0f, 0.0f, 0.0f);
        data.cubeNormalData[23] = glm::vec3( 1.0f, 0.0f, 0.0f);

        data.cubeVertexBufferBase = new EdgeVertex[data.MAX_VERTICES];

        for (uint32_t i = 0; i < data.MAX_TEXTURE_SLOTS - 1; i++)
        {
            texSlots[i] = i;
        }

        std::array<RenderData3D::Light, 10> lights;
        data.storageBuffer = StorageBuffer::CreateBuffer(1);

        data.storageBuffer->SetData(lights.data(), lights.size(), sizeof(RenderData3D::Light));
    }

    void Renderer3D::Shutdown()
    {
        delete[] data.cubeVertexBufferBase;
    }

    void Renderer3D::ResizeWindow(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewPort(0, 0, width, height);
    }

    void Renderer3D::BeginRender()
    {
        RenderCommand::EnableDepthTesting(true);
        

        StartBatch();
    }

    void Renderer3D::EndRender()
    {
        Render();
    }

    void Renderer3D::StartBatch()
    {
        data.cubeElementCount = 0;
        data.cubeVertexBufferPtr = data.cubeVertexBufferBase;
        data.cubeTextureSlotIndex = 0;
    }

    void Renderer3D::NextBatch()
    {
        Render();
        StartBatch();
    }

    void Renderer3D::Render()
    {
        const SharedRenderData& sharedData = RenderCommand::sharedData;
        sharedData.cameraUniformBuffer->Bind();
	    if (data.cubeElementCount)
	    {
            const uint32_t dataSize = (uint32_t)((uint8_t*)data.cubeVertexBufferPtr - (uint8_t*)data.cubeVertexBufferBase);
            data.cubeVertexBuffer->AddData(data.cubeVertexBufferBase, dataSize);
            RenderCommand::GetStats().dataSize += dataSize;

            //bind textures
            for (uint32_t i = 0; i < data.cubeTextureSlotIndex; i++)
                data.cubeTextureSlots[i]->Bind(i);

            data.storageBuffer->Bind();

            data.edgeGeometryShader->Bind();
            data.edgeGeometryShader->UploadIntArray("uTexture", data.MAX_TEXTURE_SLOTS - 1, texSlots);
            data.edgeGeometryShader->UploadVec4f("uLightColor", glm::vec4(1.0f));
            RenderCommand::DrawElements(data.cubeVertexArray, data.cubeElementCount);

            data.storageBuffer->Unbind();
            data.edgeGeometryShader->Unbind();
            RenderCommand::GetStats().drawCalls++;

            //unbind textures
            for (uint32_t i = 0; i < data.cubeTextureSlotIndex; i++)
                data.cubeTextureSlots[i]->Unbind();
	    }
    }

    void Renderer3D::DrawCube(const EdgeRenderData3D& renderData)
    {
        const uint32_t cubeVertexCount = 24;

        glm::mat4 transform = renderData.transform.GetMatrix();

        if (data.cubeElementCount >= data.MAX_ELEMENTS)
        {
            NextBatch();
        }

        int texIndex = -1;
        if (renderData.texture != nullptr)
        {
            for (uint32_t i = 0; i < data.cubeTextureSlotIndex; i++)
            {
                if (*data.cubeTextureSlots[i] == *renderData.texture)
                {
                    texIndex = i;
                    break;
                }
            }

            if (texIndex == -1)
            {
                if (data.cubeTextureSlotIndex >= data.MAX_TEXTURE_SLOTS)
                    NextBatch();

                texIndex = data.cubeTextureSlotIndex;
                data.cubeTextureSlots[data.cubeTextureSlotIndex] = renderData.texture;
                data.cubeTextureSlotIndex++;
            }
        }

        for (int i = 0; i < cubeVertexCount; i++)
        {
            data.cubeVertexBufferPtr->position = transform * glm::vec4(data.cubePositionData[i], 1.0f);
            data.cubeVertexBufferPtr->color = renderData.color;
            data.cubeVertexBufferPtr->isLightSource = renderData.isLightSource;
            data.cubeVertexBufferPtr->normal = data.cubeNormalData[i];
            data.cubeVertexBufferPtr->texCoords = renderData.texCoords[i % 4];
            data.cubeVertexBufferPtr->tilingFactor = renderData.tilingFactor;
            data.cubeVertexBufferPtr->texIndex = texIndex;
            data.cubeVertexBufferPtr->coreID = renderData.coreID;
            data.cubeVertexBufferPtr++;

            RenderCommand::GetStats().vertexCount++;
        }

        data.cubeElementCount += 36;

        RenderCommand::GetStats().elementCount += 36;
        RenderCommand::GetStats().objectCount++;
    }
}
