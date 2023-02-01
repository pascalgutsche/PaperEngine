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
        glVertexAttribPointer(3, TEX_ID_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*)TEX_ID_OFFSET);
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void RenderBatch::updateTextures() {
        textures.erase(textures.begin(), textures.end());
        for (int i = 0; i < numSprites; i++) {
            //if (sprites[i]->getTexture() != nullptr && !hasTexture(sprites[i]->getTexture())) {
            //    textures.insert(textures.end(), sprites[i]->getTexture());
            //}
        }
    }

    void RenderBatch::render() {
        // set spriterenderer to sprites
        // and if there are changes, display them to the renderer
        // after the vertex array has been updated, setClean to say that there are no more changes for now
        // this is needed in order to see the changes
        bool reloadVertexArray = true;
        //for (RenderData* data: dataBlocks) {
        //    if (data->dirty) {
        //        updateTextures();
        //        updateVertexProperties(data);
        //        reloadVertexArray = true;
        //    }
        //}
        // reload the vertex array if there have been made changes
        if (reloadVertexArray) {
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(float) * elements.size(), elements.data());
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

        int* texArray = new int[textures.size()];
        for (int i = 0; i < textures.size(); i++)
        {
            // activate all created textures
            textures[i]->bind(i);
            texArray[i] = i;
        }
        // upload the texture array into the shader
        // convert vector to array in order to pass the parameters through
        shader->uploadIntArray("uTexture", textures.size(), texArray);
        delete[] texArray;

#ifdef BUILD_DEBUG
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
#endif

        glBindVertexArray(vaoID);
        // draw both (with coords and color)
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        // 6 = 6 points for 2 triangles
        glDrawElements(GL_TRIANGLES, this->elements.size(), GL_UNSIGNED_INT, nullptr);
        draw_calls++;

        // stop drawing and disable array (finish it off)
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);

        // unbind everything

        glBindVertexArray(0);
        for (int i = 0; i < textures.size(); i++)
        {
            textures[i]->unbind();
        }
        shader->detach();
    }

    void RenderBatch::addVertexProperties(RenderData* renderData)
    {
        //TEXTURES
        for (int i = 0; i < renderData->textures.size(); i++) {
            std::vector<Shr<Texture>>::iterator textureIndex = std::find(this->textures.begin(), this->textures.end(), renderData->textures[i]);
            if (textureIndex == this->textures.end()) {
                textureIndex = this->textures.insert(this->textures.end(), renderData->textures.begin(), renderData->textures.end());
            }
            
            for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
            {
                if (renderData->vertices[j] == i)
                {
                    renderData->vertices[j] += textureIndex - this->textures.begin();
                }
            }
        }

        //VBO
        vertices.insert(vertices.end(), renderData->vertices.begin(), renderData->vertices.end());

        //EBO
        const int offset = dataBlocks.size() * renderData->vertices.size() / VERTEX_SIZE;

        for (int i = 0; i < renderData->ebo.size(); i++)
        {
            renderData->ebo[i] += offset;
        }
        // append vector
        elements.insert(elements.end(), renderData->ebo.begin(), renderData->ebo.end());

        dataBlocks.emplace_back(renderData);
    }

    void RenderBatch::updateVertexProperties(RenderData* renderData)
    {
        //TEXTURES
        for (int i = 0; i < renderData->textures.size(); i++) {
            std::vector<Shr<Texture>>::iterator textureIndex = std::find(this->textures.begin(), this->textures.end(), renderData->textures[i]);
            if (textureIndex == this->textures.end()) {
                textureIndex = this->textures.insert(this->textures.end(), renderData->textures.begin(), renderData->textures.end());
            }

            for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
            {
                if (renderData->vertices[j] == i)
                {
                    renderData->vertices[j] += textureIndex - this->textures.begin();
                }
            }
        }

        //VBO
        vertices.insert(vertices.end(), renderData->vertices.begin(), renderData->vertices.end());

        //EBO
        const int offset = dataBlocks.size() * renderData->vertices.size() / VERTEX_SIZE;

        for (int i = 0; i < renderData->ebo.size(); i++)
        {
            renderData->ebo[i] += offset;
        }
        // append vector
        elements.insert(elements.end(), renderData->ebo.begin(), renderData->ebo.end());

        dataBlocks.emplace_back(renderData);
    }



    void RenderBatch::removeVertexProperties(RenderData* renderData) {
        std::vector<RenderData*>::iterator renderDataSlot = std::find(dataBlocks.begin(), dataBlocks.end(), renderData);
        int verticesSlot = 0;
        for (int i = 0; i < dataBlocks.begin() - renderDataSlot; i++) {
            verticesSlot += dataBlocks[i]->vertices.size();
        }

        for (int i = verticesSlot; i < renderData->vertices.size(); i++) {
            this->vertices.erase(this->vertices.begin() + verticesSlot);
        }

        dataBlocks.erase(renderDataSlot);
    }

    bool RenderBatch::hasRoom() {
        return hasRoom_bool;
    }

    bool RenderBatch::hasTextureRoom() {
        return textures.size() < 1;
    }

    bool RenderBatch::hasTexture(Shr<Texture> tex) {
        // check if the desired texture is being used in the current renderbatch
        for (auto& texture : textures) {
            if (tex == texture) {
                return true;
            }
        }
        return false;
    }

}
