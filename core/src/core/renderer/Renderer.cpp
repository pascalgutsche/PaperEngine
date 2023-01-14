#include "_Core.h"

#include "renderer/Renderer.h"
#include "component/SpriteRenderer.h"
#include "generic/Application.h"

#include "glad/glad.h"
namespace core {

    struct less_than_key
    {
        inline bool operator() (RenderBatch* batch1, RenderBatch* batch2)
        {
            return (batch1->getZIndex() < batch2->getZIndex());
        }
    };

    Renderer::Renderer() {
        properties.width = 1080;
        properties.height = 720;
        frame_buffer = new FrameBuffer(properties);
    }

    Renderer::~Renderer() {

    }

    void Renderer::add(GameObject* gameObject) {
        // create spriterenderer with the coponent spriterneder if a spriterenderer exists
        SpriteRenderer* spriteRenderer = (SpriteRenderer*)gameObject->getComponent(std::string("sprite_renderer"));
        if (spriteRenderer != nullptr) {
            add(spriteRenderer);
        }
    }

    void Renderer::add(SpriteRenderer* spriteRenderer) {
        // add sprite to current batch if the batch has room for it and the ZIndex fits accordingly (if it is equal to both of the Zindex's (just for security))
        // do this for every other spriterenderer
        bool added = false;
        for (int i = 0; i < batches.size(); i++) {
            if (batches[i]->hasRoom() && core::GameObject::CGMap[spriteRenderer]->getZIndex() == batches[i]->getZIndex())
            {
                Texture* texture = spriteRenderer->getTexture();
                if (texture == nullptr || (batches[i]->hasTexture(texture) || batches[i]->hasTextureRoom()))
                {
                    batches[i]->addSprite(spriteRenderer);
                    added = true;
                    break;
                }
            }
        }
        if (!added) {
            // if there is no place for the spriterenderer, create a new renderbatch with the needed setup (functions)
            RenderBatch* newBatch = new RenderBatch(MAX_BATCH_SIZE, core::GameObject::CGMap[spriteRenderer]->getZIndex(), core::GameObject::CGMap[spriteRenderer]->displayMode);
            newBatch->start();
            batches.push_back(newBatch);
            newBatch->addSprite(spriteRenderer);
            std::sort(batches.begin(), batches.end(), less_than_key());
        }
    }

    void Renderer::render(const float dt) {
        glViewport(0, 0, Application::getWindow()->getWidth(), Application::getWindow()->getHeight());
        if (Application::GetImGuiEnabled())
        {
            frame_buffer->Bind();
        }

        //calculating camera vectors
        Application::getCurrentScene()->getCamera()->calcCameraVectors();

        //update GameObjects
        updateGameObjects(dt, Application::getCurrentScene()->gameObjects);

        // render all batches
        for (int i = 0; i < batches.size(); i++) {
            batches[i]->render();
        }

        frame_buffer->Unbind();
    }

    void Renderer::updateGameObjects(float deltaTime, std::vector<GameObject*>& gameObjects)
    {
        // update the gameObjects so it displays the changes

        for (auto& gameObject : gameObjects)
        {
            gameObject->update(deltaTime);
        }
    }

}