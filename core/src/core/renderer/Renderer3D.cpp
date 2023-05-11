#include "_Core.h"

#include "renderer/Renderer3D.h"
#include "renderer/RenderCommand.h"
#include "utils/DataPool.h"
#include "renderer/Shader.h"
#include "generic/Application.h"
#include "imgui/ImGuiLayer.h"

namespace core {

    struct EdgeVertex
    {
        glm::vec3 position;
        glm::vec4 color;
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

        Shr<Camera> camera;

        glm::vec4 cubeVertexData[24];

        Renderer3D::Stats stats;

    };

    static RenderData3D data;
    static int texSlots[data.MAX_TEXTURE_SLOTS - 1];

    void Renderer3D::Init()
    {
        RenderCommand::Init();

        BufferLayout edgeGeometryLayout = {
            { GLSLDataType::FLOAT3, "aPos" },
            { GLSLDataType::FLOAT4, "aColor" },

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

        //back
        data.cubeVertexData[0] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
        data.cubeVertexData[1] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
        data.cubeVertexData[2] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
        data.cubeVertexData[3] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);

        //front
        data.cubeVertexData[4] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
        data.cubeVertexData[5] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
        data.cubeVertexData[6] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        data.cubeVertexData[7] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);

        //bottom
    	data.cubeVertexData[8] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
        data.cubeVertexData[9] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
        data.cubeVertexData[10] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
        data.cubeVertexData[11] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);

        //top
        data.cubeVertexData[12] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
        data.cubeVertexData[13] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
        data.cubeVertexData[14] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        data.cubeVertexData[15] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);

        //left
        data.cubeVertexData[16] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
        data.cubeVertexData[17] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
        data.cubeVertexData[18] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
        data.cubeVertexData[19] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);

        //right
        data.cubeVertexData[20] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
        data.cubeVertexData[21] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
        data.cubeVertexData[22] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
        data.cubeVertexData[23] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);


        data.cubeVertexBufferBase = new EdgeVertex[data.MAX_VERTICES];

        for (uint32_t i = 0; i < data.MAX_TEXTURE_SLOTS - 1; i++)
        {
            texSlots[i] = i;
        }
    }

    void Renderer3D::Shutdown()
    {
        delete[] data.cubeVertexBufferBase;
    }

    void Renderer3D::ResizeWindow(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewPort(0, 0, width, height);
    }

    void Renderer3D::BeginRender(Shr<Camera> camera)
    {
        data.camera = camera;
        data.camera->CalcCameraVectors();
        
        RenderCommand::GetFramebuffer()->Bind();

        RenderCommand::EnableDepthTesting(true);
        

        StartBatch();
    }

    void Renderer3D::EndRender()
    {
        Render();
        RenderCommand::GetFramebuffer()->Unbind();
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
	    if (data.cubeElementCount)
	    {
            const uint32_t dataSize = (uint32_t)((uint8_t*)data.cubeVertexBufferPtr - (uint8_t*)data.cubeVertexBufferBase);
            data.cubeVertexBuffer->AddData(data.cubeVertexBufferBase, dataSize);
            data.stats.dataSize += dataSize;

            //bind textures
            for (uint32_t i = 0; i < data.cubeTextureSlotIndex; i++)
                data.cubeTextureSlots[i]->Bind(i);

            //data.framebuffer->BindAttachmentAsTexture(1, data.MAX_TEXTURE_SLOTS - 1);

            data.edgeGeometryShader->Bind();
            data.edgeGeometryShader->UploadMat4f("uPerspective", data.camera->GetProjectionMatrix());
            data.edgeGeometryShader->UploadMat4f("uOrthographic", data.camera->GetOrthographicMatrix());
            data.edgeGeometryShader->UploadMat4f("uView", data.camera->GetViewMatrix());
            data.edgeGeometryShader->UploadMat4f("uModel", glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            data.edgeGeometryShader->UploadIntArray("uTexture", data.MAX_TEXTURE_SLOTS - 1, texSlots);
            //data.edgeGeometryShader->UploadInt("uIDAttachment", data.MAX_TEXTURE_SLOTS - 1);
            //data.edgeGeometryShader->UploadVec2f("screenSize", glm::vec2(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight()));
            RenderCommand::DrawElements(data.cubeVertexArray, data.cubeElementCount);
            data.edgeGeometryShader->Unbind();
            data.stats.drawCalls++;

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
            data.cubeVertexBufferPtr->position = transform * data.cubeVertexData[i];
            data.cubeVertexBufferPtr->color = renderData.color;
            data.cubeVertexBufferPtr->texCoords = renderData.texCoords[i % 4];
            data.cubeVertexBufferPtr->tilingFactor = renderData.tilingFactor;
            data.cubeVertexBufferPtr->texIndex = texIndex;
            data.cubeVertexBufferPtr->coreID = renderData.coreID;
            data.cubeVertexBufferPtr++;

            data.stats.vertexCount++;
        }

        data.cubeElementCount += 36;

        data.stats.elementCount += 36;
        data.stats.objectCount++;
    }

    Renderer3D::Stats Renderer3D::GetStats()
    {
        return data.stats;
    }

    void Renderer3D::ClearStats()
    {
        memset(&data.stats, 0, sizeof(Stats));
    }
}
