#include "_Core.h"

#include "renderer/Renderer.h"
#include "component/SpriteRenderer.h"
#include "generic/Application.h"
#include "event/Input.h"
#include "event/MouseCodes.h"
#include "event/GameEvent.h"
#include "renderer/RenderCommand.h"

namespace core {

    struct RectangleVertex
    {
        glm::vec2 position;
        glm::vec4 color;
        glm::vec2 texCoords;
        int texIndex;
        float tilingFactor;
        int coreID;
    };

    struct TriangleVertex
    {
        glm::vec2 position;
        glm::vec4 color;
        glm::vec2 texCoords;
        int texIndex;
        float tilingFactor;
        int coreID;
    };

    struct RenderData
    {
        static constexpr uint32_t MAX_VERTICES = 40000;
        static constexpr uint32_t MAX_ELEMENTS = 60000;
        static constexpr uint32_t MAX_TEXTURE_SLOTS = 32;

        Shr<Shader> edgeGeometryShader;
        Shr<VertexArray> rectangleVertexArray;
        Shr<VertexBuffer> rectangleVertexBuffer;

        Shr<VertexArray> triangleVertexArray;
        Shr<VertexBuffer> triangleVertexBuffer;

        uint32_t rectangleElementCount = 0;
        RectangleVertex* rectangleVertexBufferBase = nullptr;
        RectangleVertex* rectangleVertexBufferPtr = nullptr;

        uint32_t triangleElementCount = 0;
        TriangleVertex* triangleVertexBufferBase = nullptr;
        TriangleVertex* triangleVertexBufferPtr = nullptr;




        Camera camera;

        glm::vec4 rectangleVertexData[4];
        glm::vec4 triangleVertexData[3];
    };

    static RenderData data;

    void Renderer::Init()
    {
        RenderCommand::Init();

        BufferLayout edgeGeometryLayout = {
            { GLSLDataType::FLOAT2, "aPos" },
            { GLSLDataType::FLOAT4, "aColor" },
            { GLSLDataType::FLOAT2, "aTexCoord" },
            { GLSLDataType::FLOAT , "aTilingFactor"},
            { GLSLDataType::FLOAT , "aTexID" },
            { GLSLDataType::INT   , "aCoreID" }
        };

        data.edgeGeometryShader = DataPool::GetShader("default"); //TODO: change name
        data.edgeGeometryShader->Compile();
        data.rectangleVertexArray = VertexArray::CreateArray();
        data.rectangleVertexBuffer = VertexBuffer::CreateBuffer(edgeGeometryLayout, data.MAX_VERTICES * sizeof(RectangleVertex));
        data.rectangleVertexArray->SetVertexBuffer(data.rectangleVertexBuffer);
        
        data.triangleVertexArray = VertexArray::CreateArray();
        data.triangleVertexBuffer = VertexBuffer::CreateBuffer(edgeGeometryLayout, data.MAX_VERTICES * sizeof(TriangleVertex));
        data.triangleVertexArray->SetVertexBuffer(data.triangleVertexBuffer);

        data.rectangleVertexBufferBase = new RectangleVertex[data.MAX_VERTICES];

        data.triangleVertexBufferBase = new TriangleVertex[data.MAX_VERTICES];

        uint32_t* rectangleElements = new uint32_t[data.MAX_ELEMENTS];

        int offsetRectangle = 0;
        for (int i = 0; i < data.MAX_ELEMENTS; i += 6)
        {
            // first triangle
            rectangleElements[i + 0] = offsetRectangle + 3;
            rectangleElements[i + 1] = offsetRectangle + 2;
            rectangleElements[i + 2] = offsetRectangle + 0;
            // second triangle               
            rectangleElements[i + 3] = offsetRectangle + 0;
            rectangleElements[i + 4] = offsetRectangle + 2;
            rectangleElements[i + 5] = offsetRectangle + 1;

            offsetRectangle += 4;
        }

        Shr<ElementBuffer> rectangleElementbuffer = ElementBuffer::CreateBuffer(rectangleElements, data.MAX_ELEMENTS);
        data.rectangleVertexArray->SetElementBuffer(rectangleElementbuffer);
        delete[] rectangleElements;

        uint32_t* triangleElements = new uint32_t[data.MAX_ELEMENTS];

        int offsetTriangle = 0;
        for (int i = 0; i < data.MAX_ELEMENTS; i += 6)
        {
            // first triangle
            triangleElements[i + 0] = offsetTriangle + 3;
            triangleElements[i + 1] = offsetTriangle + 2;
            triangleElements[i + 2] = offsetTriangle + 0;
            // second triangle              
            triangleElements[i + 3] = offsetTriangle + 0;
            triangleElements[i + 4] = offsetTriangle + 2;
            triangleElements[i + 5] = offsetTriangle + 1;

            offsetTriangle += 4;
        }

        Shr<ElementBuffer> triangleElementbuffer = ElementBuffer::CreateBuffer(triangleElements, data.MAX_ELEMENTS);
        data.triangleVertexArray->SetElementBuffer(triangleElementbuffer);
        delete[] triangleElements;

        data.rectangleVertexData[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
        data.rectangleVertexData[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
        data.rectangleVertexData[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
        data.rectangleVertexData[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);

        data.triangleVertexData[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
        data.triangleVertexData[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
        data.triangleVertexData[2] = glm::vec4( 0.0f,  0.5f, 0.0f, 1.0f);
    }



    void Renderer::Shutdown()
    {
        delete[] data.rectangleVertexBufferBase;
        delete[] data.triangleVertexBufferBase;
    }

    void Renderer::BeginRender(const Camera& camera)
    {
        data.camera = camera;
        data.camera.calcCameraVectors();

        StartBatch();
    }

    void Renderer::EndRender()
    {
        Render();
    }

    void Renderer::StartBatch()
    {
        data.rectangleElementCount = 0;
        data.rectangleVertexBufferPtr = data.rectangleVertexBufferBase;

        data.triangleElementCount = 0;
        data.triangleVertexBufferPtr = data.triangleVertexBufferBase;
    }

    void Renderer::NextBatch()
    {
        Render();
        StartBatch();
    }


    void Renderer::Render()
    {
	    if (data.rectangleElementCount)
	    {
            const uint32_t dataSize = (uint32_t)((uint8_t*)data.rectangleVertexBufferPtr - (uint8_t*)data.rectangleVertexBufferBase);
            data.triangleVertexBuffer->AddData(data.rectangleVertexBufferBase, dataSize);

            data.edgeGeometryShader->Bind();
            data.edgeGeometryShader->UploadMat4f("uProjection", data.camera.getProjectionMatrix());
            data.edgeGeometryShader->UploadMat4f("uView", data.camera.getViewMatrix());
            RenderCommand::DrawElements(data.triangleVertexArray, data.rectangleElementCount);
	    }

        if (data.triangleElementCount)
        {
            const uint32_t dataSize = (uint32_t)((uint8_t*)data.triangleVertexBufferPtr - (uint8_t*)data.triangleVertexBufferBase);
            data.triangleVertexBuffer->AddData(data.triangleVertexBufferBase, dataSize);

            data.edgeGeometryShader->Bind();
            data.edgeGeometryShader->UploadMat4f("uProjection", data.camera.getProjectionMatrix());
            data.edgeGeometryShader->UploadMat4f("uView", data.camera.getViewMatrix());
            RenderCommand::DrawElements(data.triangleVertexArray, data.triangleElementCount);
        }
    }

    void Renderer::DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec4 color, core_id coreID)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f))
            * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

        DrawRectangle(transform, color, coreID);
    }

    void Renderer::DrawRectangle(glm::mat4 transform, glm::vec4 color, core_id coreID)
    {
        const uint32_t rectanleVertexCount = 4;
        const float texIndex = -1.0f;
        const glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float tilingFactor = 1.0f;

        if (data.rectangleElementCount >= data.MAX_ELEMENTS)
        {
            NextBatch();
        }

        for (int i = 0; i < rectanleVertexCount; i++)
        {
            data.rectangleVertexBufferPtr->position = transform * data.rectangleVertexData[i];
            data.rectangleVertexBufferPtr->color = color;
            data.rectangleVertexBufferPtr->texCoords = texCoords[i];
            data.rectangleVertexBufferPtr->tilingFactor = tilingFactor;
            data.rectangleVertexBufferPtr->texIndex = texIndex;
            data.rectangleVertexBufferPtr->coreID = coreID;
            data.rectangleVertexBufferPtr++;
        }

        data.rectangleElementCount += 6;
    }


    /*
    Renderer* Renderer::instance;

    struct less_than_key
    {
        inline bool operator() (RenderBatch* batch1, RenderBatch* batch2)
        {
            return (batch1->GetZIndex() < batch2->GetZIndex());
        }
    };

    Renderer::Renderer() {
        instance = this;
        FramebufferAttachSpecification attach = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::DEPTH24STECIL8 };
        properties.attachment = attach;
        properties.width = 1080;
        properties.height = 720;
        framebuffer = Framebuffer::CreateBuffer(properties);
    }

    Renderer::~Renderer() {

    }

    void Renderer::add(RenderData* renderData)
    {
        // add vertices to renderbatch
        // if there is no existing renderbatch, create one 
        bool found = false;
        for (auto g : batches)
        {
            if (renderData->zIndex == g->GetZIndex() && renderData->projectionMode == g->GetProjectionMode())
            {
                found = true;
                g->addVertexProperties(renderData);
            }
        }
        if (!found)
        {
            RenderBatch* renderBatch = new RenderBatch(renderData->zIndex, renderData->projectionMode);
            batches.emplace(batches.end(), renderBatch);
            renderBatch->start();

            renderBatch->addVertexProperties(renderData);
        }
    }

    void Renderer::remove(RenderData* renderData)
    {
        for (auto g : batches)
        {
            g->removeVertexProperties(renderData);
        }
    }

    void Renderer::render(const float dt) {
        framebuffer->Bind();
        if (!Application::GetImGuiEnabled())
        {
            if (framebuffer->GetSpecification().width != Application::GetWindow()->GetWidth() || framebuffer->GetSpecification().height != Application::GetWindow()->GetHeight())
            {
                framebuffer->Resize(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());
            }
            glViewport(0, 0, Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());
        }

        //calculating camera vectors
        Application::GetActiveScene()->GetCamera()->calcCameraVectors();

        //update GameObjects
        updateGameObjects(dt);

        //clear core id attachment to -1
        framebuffer->ClearAttachment(1, -1);

        // render all batches
        for (int i = 0; i < batches.size(); i++) {
            if (batches[i]->render())
            {
                delete batches[i];
                batches.erase(batches.begin() + i);
            }
            
        }

        //IMGUI window panel for framebuffer picture
        if (!Application::GetImGuiEnabled()) {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
            window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;// | ImGuiWindowFlags_MenuBar;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGuiViewport& viewport = *ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport.Pos);
            ImGui::SetNextWindowSize(viewport.Size);
            ImGui::SetNextWindowViewport(viewport.ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            ImGui::Begin(" ", nullptr, window_flags);
            ImGui::PopStyleVar(3);

            ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
            if (viewportSize != *(glm::vec2*)&viewport_panel_size)
            {
                viewportSize = { viewport_panel_size.x, viewport_panel_size.y };
                framebuffer->Resize(viewportSize.x, viewportSize.y);
            }
            uint32_t textureID = framebuffer->GetColorID(0);

            ImGui::Image((void*)textureID, ImVec2(viewportSize.x, viewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::End();
        }

        //mouse picking;
        glm::ivec2 pos = glm::ivec2(-1, -1);

        if (!Application::GetImGuiEnabled())
        {
            glm::vec2 mousePos = Input::GetMousPos();

            mousePos.y = Application::GetWindow()->GetHeight() - mousePos.y;
            if (mousePos.x < Application::GetWindow()->GetWidth() && mousePos.y < Application::GetWindow()->GetHeight())
            {
                pos = mousePos;
            }

        }
        else if (Application::GetImGuiLayer().IsMouseInsideViewport())
        {
            pos = Application::GetImGuiLayer().GetMousePosViewportRelative();
        }

        if (pos.x >= 0 && pos.y >= 0) {
            mouseHoverID[0] = framebuffer->ReadPixel(1, pos);
        }

        if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mouseClickedID[0] = mouseHoverID[0];
            if (mouseClickedID[0] != mouseClickedID[1] && mouseClickedID[0] != -1 && !pressed) 
            {
                Application::QueueEvents(new GameObjectPressedEvent(GameObject::GetGameObjectByID(mouseClickedID[0])));
            }
            pressed = true;
            mouseClickedID[1] = mouseClickedID[0];
        }
        else {
            pressed = false;
            if (mouseClickedID[1] != -1) {
                Application::QueueEvents(new GameObjectReleasedEvent(GameObject::GetGameObjectByID(mouseClickedID[1])));
            }


            mouseClickedID[1] = -1;
            if (mouseHoverID[0] != mouseHoverID[1])
            {
                
                if (mouseHoverID[1] != -1) {
                    Application::QueueEvents(new GameObjectHoverEndEvent(GameObject::GetGameObjectByID(mouseHoverID[1])));
                }
                if (mouseHoverID[0] != -1) {
                    Application::QueueEvents(new GameObjectHoverBeginEvent(GameObject::GetGameObjectByID(mouseHoverID[0])));
                }
					
            }
            else if (mouseHoverID[0] != mouseHoverID[1] && mouseHoverID[0] == -1)
            {
            }
            mouseHoverID[1] = mouseHoverID[0];
        }


        framebuffer->Unbind();
    }

    void Renderer::updateGameObjects(float dt)
    {
        // update the gameObjects so it displays the changes

        for (Layer* layer : Application::GetLayerStack())
        {
            for (GameObject* game_object : layer->GetGameObjects())
            {
                if (!game_object->IsRunning()) continue;
                game_object->update(dt);
            }
        }
    }
    */

    
}
