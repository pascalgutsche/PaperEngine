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
            return (batch1->getZIndex() < batch2->getZIndex());
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
            if (renderData->zIndex == g->getZIndex() && renderData->displayMode == g->getDisplayMode())
            {
                found = true;
                g->addVertexProperties(renderData->ebo, renderData->vertices, renderData->textures);
            }
        }
        if (!found)
        {
            RenderBatch* renderBatch = new RenderBatch(1000, renderData->zIndex, renderData->displayMode);
            batches.emplace(batches.end(), renderBatch);

            renderBatch->addVertexProperties(renderData->ebo, renderData->vertices, renderData->textures);
        }
    }

  
    void Renderer::render(LayerStack& layer_stack, const float dt) {
        if (Application::GetImGuiEnabled())
			frame_buffer->Bind();
        else
            glViewport(0, 0, Application::getWindow()->getWidth(), Application::getWindow()->getHeight());

        //calculating camera vectors
        Application::getCurrentScene()->getCamera()->calcCameraVectors();

        //update GameObjects
        updateGameObjects(dt, Application::getCurrentScene()->gameObjects);

        int sprites_count_calc = 0;
        int vertices_count_calc = 0;
        // render all batches
        for (int i = 0; i < batches.size(); i++) {
            sprites_count_calc += batches[i]->GetSpritesCount();
            vertices_count_calc += batches[i]->GetVertexCount();
            batches[i]->render();
        }

        sprites_count = sprites_count_calc;
        vertex_count = vertices_count_calc;

        frame_buffer->Unbind();
    }

    void Renderer::updateGameObjects(float dt, std::vector<GameObject*>& gameObjects)
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
}
