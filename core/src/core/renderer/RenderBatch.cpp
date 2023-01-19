#include "_Core.h"

#include "renderer/RenderBatch.h"
#include "utils/DataPool.h"
#include "generic/Window.h"
#include "component/SpriteRenderer.h"
#include "generic/GameObject.h"
#include "generic/Application.h"

#include <GLAD/glad.h>

namespace core {

    RenderBatch::RenderBatch(int maxBatchSize, int zIndex, DataPool::DISPLAYMODE displaymode)
    {
        // set local and current values
        this->zIndex = zIndex;
        this->maxBatchSize = maxBatchSize;
        this->displayMode = displaymode;
        numSprites = 0;

        // menu gui mode needs a special shader because of uProjection is aPos basically
        if (displaymode == DataPool::DISPLAYMODE::NONE)
        {
            shader = DataPool::getShader("menu");
        }
        else
        {
            // orthographic and and perspective camera modes can be utlilized by the default shader
            shader = DataPool::getShader("default");
        }

        shader->compile();
        RenderBatch::hasRoom_bool = true;

        // we do not need this because the SPRITERENDERER does it for us
        //loadVertexProperties(0);
    }

    RenderBatch::~RenderBatch() 
    {

    };

    void RenderBatch::start() {
        // generate vertex buffer
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);

        // allocate space for vertex array
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        // put everything from the vector into the array, so that we can access it from within gl functions || using vectors makes it easier for dynamic allocations
        // create task for gpu and save data
        glBufferData(GL_ARRAY_BUFFER, maxBatchSize * VERTEX_SIZE_BYTES * 4, vertices.data(), GL_DYNAMIC_DRAW);
        //element buffer object (useful for creating squares)

        glGenBuffers(1, &eboID);
        // create array
        
        // use the array
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * maxBatchSize * 6, elements.data(), GL_DYNAMIC_DRAW);

        // saving / uploading points (x,y) to vertex slot 0
        //declaration of vertex basically (start at byte offset x, array index y,...)
        // POSITION
        glVertexAttribPointer(0, POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*)POS_OFFSET);
        glEnableVertexAttribArray(0);

        // saving / uploading color into vertex, slot 1, 4 parameters, and start at the offset of 8 bytes (array index 2)
        // declaration of vertex
        // COLOR
        glVertexAttribPointer(1, COLOR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*)COLOR_OFFSET);
        glEnableVertexAttribArray(1);

        // 1. slot  2. attribute number 3. type  4. normalization    5. sizeof a vertex (one line) 6. byte offset
        // TEXTURE_COORDINATES
        glVertexAttribPointer(2, TEX_COORDS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*)TEX_COORDS_OFFSET);
        glEnableVertexAttribArray(2);

        //TEXTURE_ID
        glVertexAttribPointer(3, TEX_ID_SIZE, GL_INT, GL_FALSE, VERTEX_SIZE_BYTES, (void*)TEX_ID_OFFSET);
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void RenderBatch::updateTextures() {
        textures.erase(textures.begin(), textures.end());
        for (int i = 0; i < numSprites; i++) {
            if (sprites[i]->getTexture() != nullptr && !hasTexture(sprites[i]->getTexture())) {
                textures.insert(textures.end(), sprites[i]->getTexture());
            }
        }
    }

    void RenderBatch::render() {
        // set spriterenderer to sprites
        // and if there are changes, display them to the renderer
        // after the vertex array has been updated, setClean to say that there are no more changes for now
        // this is needed in order to see the changes
        bool reloadVertexArray = false;
        for (int i = 0; i < numSprites; i++) {
            SpriteRenderer* spriteRenderer = sprites[i];
            if (spriteRenderer->getIsDirty()) {
                updateTextures();

                spriteRenderer->setClean();
                reloadVertexArray = true;
            }
        }
        // reload the vertex array if there have been made changes
        if (reloadVertexArray) {
            // updates the vertex array in order to see the changes within rendering
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, maxBatchSize * VERTEX_SIZE_BYTES * 4, vertices.data());
        }

        // use the shader and upload the shader variables
        shader->use();

        if (displayMode == DataPool::DISPLAYMODE::ORTHOGRAPHIC)
        {
            shader->uploadMat4f("uProjection", Application::getCurrentScene()->getCamera()->getOrthographicMatrix());
        }
        else if (displayMode == DataPool::DISPLAYMODE::PERSPECTIVE)
        {
            shader->uploadMat4f("uProjection", Application::getCurrentScene()->getCamera()->getProjectionMatrix());
        }
        else if (displayMode == DataPool::DISPLAYMODE::NONE)
        {
            shader->uploadVec2f("uProjection", glm::vec2(vertices[POS_OFFSET], vertices[POS_OFFSET + 1]));
        }
        // fov
        shader->uploadMat4f("uView", Application::getCurrentScene()->getCamera()->getViewMatrix());

        for (int i = 0; i < textures.size(); i++)
        {
            // activate all created textures
            textures[i]->bind(i);
        }
        // upload the texture array into the shader
        // convert vector to array in order to pass the parameters through
        int* texArray = new int[texSlots.size()];
        std::copy(texSlots.begin(), texSlots.end(), texArray);
        shader->uploadIntArray("uTexture", texSlots.size(), texArray);
        delete texArray;

#ifdef BUILD_DEBUG
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
#endif

        glBindVertexArray(vaoID);
        // draw both (with coords and color)
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // 6 = 6 points for 2 triangles
        glDrawElements(GL_TRIANGLES, this->numSprites * 6, GL_UNSIGNED_INT, nullptr);
        draw_calls++;

        // stop drawing and disable array (finish it off)
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // unbind everything

        glBindVertexArray(0);
        for (int i = 0; i < textures.size(); i++)
        {
            textures[i]->unbind();
        }
        shader->detach();
    }

    void RenderBatch::addVertexProperties(std::vector<float> verticesData, std::vector<Texture*> textures)
    {
        this->textures.insert(this->textures.end(), textures.begin(), textures.end());

        verticesData.emplace()

        vertices.insert(vertices.end(), verticesData.begin(), verticesData.end());
    }

    void RenderBatch::addElementIndices(std::vector<unsigned int> ebo)
    {
        // append vector
        elements.insert(elements.end(), ebo.begin(), ebo.end());
    }

    bool RenderBatch::hasRoom() {
        return hasRoom_bool;
    }

    bool RenderBatch::hasTextureRoom() {
        return textures.size() < 1;
    }

    bool RenderBatch::hasTexture(std::shared_ptr<Texture> tex) {
        // check if the desired texture is being used in the current renderbatch
        for (auto& texture : textures) {
            if (tex == texture) {
                return true;
            }
        }
        return false;
    }

    int RenderBatch::getZIndex() {
        // do you really need an explanation for this
        return this->zIndex;
    }

    int RenderBatch::GetSpritesCount() const
    {
        return numSprites;
    }

    int RenderBatch::GetVertexCount() const
    {
        return vertices.size() / VERTEX_SIZE;
    }
}
