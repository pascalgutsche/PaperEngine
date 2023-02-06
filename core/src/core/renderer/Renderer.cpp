#include "_Core.h"

#include "renderer/Renderer.h"
#include "component/SpriteRenderer.h"
#include "generic/Application.h"

#include "glad/glad.h"
namespace core {

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
        properties.width = 1080;
        properties.height = 720;
        frame_buffer = new FrameBuffer(properties);
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
            if (renderData->zIndex == g->GetZIndex() && renderData->displayMode == g->GetDisplayMode())
            {
                found = true;
                g->addVertexProperties(renderData);
            }
        }
        if (!found)
        {
            RenderBatch* renderBatch = new RenderBatch(renderData->zIndex, renderData->displayMode);
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
        if (Application::GetImGuiEnabled())
			frame_buffer->Bind();
        else
            glViewport(0, 0, Application::getWindow()->getWidth(), Application::getWindow()->getHeight());

        //calculating camera vectors
        Application::GetActiveScene()->GetCamera()->calcCameraVectors();

        //update GameObjects
        updateGameObjects(dt);

        texturesInUse.clear();
        int vertices_count_calc = 0;
        // render all batches
        for (int i = 0; i < batches.size(); i++) {
            vertices_count_calc += batches[i]->GetVertexCount();
            std::vector<Shr<Texture>> textures = batches[i]->GetTexturesInUse();
            texturesInUse.insert(texturesInUse.end(), textures.begin(), textures.end());
            if (batches[i]->render())
            {
                delete batches[i];
                batches.erase(batches.begin() + i);
            }
            
        }
        vertex_count = vertices_count_calc;

        frame_buffer->Unbind();
    }

    void Renderer::updateGameObjects(float dt)
    {
        // update the gameObjects so it displays the changes

        for (Layer* layer : Application::GetLayerStack())
        {
            for (GameObject* game_object : layer->GetGameObjects())
            {
                game_object->update(dt);
            }
        }
    }

    uint32_t Renderer::RequestID()
    {
        uint32_t id = leastAvailableId;
        idInUse.emplace_back(leastAvailableId);
        leastAvailableId = idInUse.size();
        return id;
    }

    void Renderer::RemoveID(uint32_t id)
    {
        std::vector<uint32_t>::iterator it = std::find(idInUse.begin(), idInUse.end(), id);
        if (it == idInUse.end())
        {
            CORE_ASSERT(false, "ID does not exist");
            return;
        }
        idInUse.erase(it);
        if (id < leastAvailableId) leastAvailableId = id;
    }

    void Renderer::Init()
    {
	    
    }

    void Renderer::Shutdown()
    {
	    
    }

    void Renderer::BeginRender(Shr<Camera>& camera)
    {
	    
    }

    void Renderer::SubmitData(Shr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawElements(vertexArray);
    }

    void Renderer::EndRender()
    {
	    
    }
}
