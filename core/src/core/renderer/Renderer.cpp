#include "_Core.h"

#include "renderer/Renderer.h"
#include "component/SpriteRenderer.h"
#include "generic/Application.h"
#include "event/Input.h"
#include "event/MouseCodes.h"
#include "event/GameEvent.h"

namespace core {

    void Renderer::Init()
    {
        RenderCommand::Init();

        BufferLayout layout = {
            { GLSLDataType::FLOAT2, "aPos" },
            { GLSLDataType::FLOAT4, "aColor" },
            { GLSLDataType::FLOAT2, "aTexCoord" },
            { GLSLDataType::FLOAT , "aTexID" }
        };

        triangleVertexArray = VertexArray::CreateArray();
        triangleVertexBuffer = VertexBuffer::CreateBuffer(layout, MAX_VERTICES * sizeof(float));
        triangleElementBuffer = ElementBuffer::CreateBuffer(MAX_ELEMENTS * sizeof(unsigned int));

        triangleVertexArray->SetVertexBuffer(triangleVertexBuffer);
        triangleVertexArray->SetElementBuffer(triangleElementBuffer);
    }

    void Renderer::Shutdown()
    {

    }


    void Renderer::SubmitData(RenderData* renderData)
    {
        if (batches.size() < renderData->zIndex + 1)
        {
            batches.resize(renderData->zIndex, std::vector<RenderData*>());
        }
        batches[renderData->zIndex].push_back(renderData);
    } 

    void Renderer::Render(Shr<Camera>& camera)
    {
        
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
