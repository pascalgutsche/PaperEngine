#include "_Core.h"

#include "renderer/RenderBatch.h"
#include "utils/DataPool.h"
#include "generic/Window.h"
#include "component/SpriteRenderer.h"
#include "generic/GameObject.h"
#include "generic/Application.h"

#include <GLAD/glad.h>

namespace core {

    RenderBatch::RenderBatch(int zIndex, DisplayMode displaymode)
    {
        // set local and current values
        this->zIndex = zIndex;
        this->displayMode = displaymode;

        // menu gui mode needs a special shader because of uProjection is aPos basically
        if (displaymode == SCREEN)
        {
            shader = DataPool::getShader("menu");
        }
        else
        {
            // orthographic and and perspective camera modes can be utlilized by the default shader
            shader = DataPool::getShader("default");
        }

        shader->compile();
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
        // create task for gpu and save data
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * VERTEX_SIZE_BYTES, vertices.data(), GL_DYNAMIC_DRAW);
        //element buffer object (useful for creating squares)

        glGenBuffers(1, &eboID);
        // create array

        // use the array
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * MAX_ELEMENT_COUNT, elements.data(), GL_DYNAMIC_DRAW);

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

    int RenderBatch::render() {
        for (RenderData* data : dataBlocks) {
            if (data->dirty) {
                updateVertexProperties(data);
                data->dirty = false;
            }
        }

        if (vertices.size() == 0) { return 1; }
        // reload the vertex array if there have been made changes
        if (reloadBufferArrays) {
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(float) * elements.size(), elements.data());
            reloadBufferArrays = false;
        }

        // use the shader and upload the shader variables
        shader->use();


        switch (displayMode)
        {
        case PERSPECTIVE:
            shader->uploadMat4f("uProjection", Application::getCurrentScene()->getCamera()->getProjectionMatrix());
            break;
        case ORTHOGRAPHIC:
            shader->uploadMat4f("uProjection", Application::getCurrentScene()->getCamera()->getOrthographicMatrix());
            break;
        case SCREEN:
            shader->uploadVec2f("uProjection", glm::vec2(vertices[POS_OFFSET], vertices[POS_OFFSET + 1]));
            break;
        }
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

        delete[] texArray;
        return 0;
    }

    void RenderBatch::addVertexProperties(RenderData* renderData)
    {
        std::vector<float> RDVerticesCopy = renderData->vertices;
        //TEXTURE
        for (int i = 0; i < renderData->textures.size(); i++) {
            std::vector<Shr<Texture>>::iterator textureIndex = std::find(this->textures.begin(), this->textures.end(), renderData->textures[i]);
            if (textureIndex == this->textures.end()) {
                textureIndex = this->textures.insert(this->textures.end(), renderData->textures.begin(), renderData->textures.end());
            }
            
            for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
            {
                if (renderData->vertices[j] == i)
                {
                    RDVerticesCopy[j] += textureIndex - this->textures.begin() + 1;
                }
            }
        }
        for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
        {
            if (renderData->vertices[j] == -1)
            {
                RDVerticesCopy[j] = 0;
            }
        }



        //VBO
        if (renderData->vertices.size() <= 0)
        {
            CORE_ASSERT(renderData->vertices.size() > 0, "invalid vbo buffer");
            return;
        }
        int renderDataVertexCount = renderData->vertices.size() / VERTEX_SIZE;
        renderData->vertexSlot = vertices.insert(vertices.end(), RDVerticesCopy.begin(), RDVerticesCopy.end()) - vertices.begin();

        //EBO
        //checking if vbo and ebo match

        if (renderData->ebo.size() <= 0)
        {
            CORE_ASSERT(renderData->ebo.size() > 0, "invalid ebo buffer");
            return;
        } 
        int highestElement = renderData->ebo.at(0);
        for (int i : renderData->ebo)
        {
	        if (i > highestElement)
	        {
                highestElement = i;
	        }
        }
        highestElement++; //increment because 0 in the element buffer is also a vertex
        if (highestElement != renderDataVertexCount)
        {
            CORE_ASSERT(highestElement == renderDataVertexCount, "invalid ebo buffer");
            return;
        }

        //convert RenderData ebo to RenderBatch ebo

        int batchVertexCount = renderData->vertexSlot / VERTEX_SIZE;
        renderData->elementSlot = elements.size();

        for (int i = 0; i < renderData->ebo.size(); i++)
        {
            this->elements.emplace_back(renderData->ebo.at(i) + batchVertexCount);
        }

        renderData->oldVertexSize = renderData->vertices.size();
        renderData->oldElementSize = renderData->ebo.size();
        renderData->oldTextures = renderData->textures;
        renderData->dataBlockSlot = dataBlocks.size();
        dataBlocks.emplace_back(renderData);
        reloadBufferArrays = true;
    }

    void RenderBatch::updateVertexProperties(RenderData* renderData)
    {
        std::vector<float> RDVerticesCopy = renderData->vertices;
        //TEXTURES
        if (renderData->textures != renderData->oldTextures)
        {
            //checking which texture was removed;
            std::vector<Shr<Texture>> toRemoveTextures;
            for (Shr<Texture> textureOld : renderData->oldTextures)
            {
                bool found = false;
	            for (Shr<Texture> textureNew : renderData->textures)
	            {
		            if (textureOld == textureNew)
		            {
                        found = true;
		            }
	            }
                if (!found)
                {
                    toRemoveTextures.emplace_back(textureOld);
                }
            }

            //check if removed textures are still beeing used
            for (int i = 0; i < toRemoveTextures.size(); i++)
            {
	            for (RenderData* rd : dataBlocks)
	            {
                    if (rd == renderData) continue;
                    if (std::find(rd->textures.begin(), rd->textures.end(), toRemoveTextures[i]) !=  rd->textures.end())
                    {
                        toRemoveTextures.erase(toRemoveTextures.begin() + i);
                        break;
                    }
	            }
            }

            //check which texture is new
            std::vector<Shr<Texture>> toAddTextures;
            for (Shr<Texture> texture : renderData->textures)
            {
	            if (std::find(this->textures.begin(), this->textures.end(), texture) == this->textures.end())
	            {
                    toAddTextures.emplace_back(texture);
	            }
            }

            if (this->textures.size() + toAddTextures.size() - toRemoveTextures.size() > 32)
            {
                removeVertexProperties(renderData);
                Application::getCurrentScene()->GetRenderer().add(renderData);
            }
            else
            {
                std::vector<Shr<Texture>> textureCopy = this->textures;
                for (int i = 0; i < toRemoveTextures.size(); i++)
                {
                    for (int j = 0; j < this->textures.size(); j++)
                    {
                        if (toRemoveTextures[i] == this->textures[j])
                        {
                            this->textures.erase(this->textures.begin() + j);
                        }
                    }
                }

                //add textures to renderbatch and change the structs to the right value
                this->textures.insert(this->textures.end(), toAddTextures.begin(), toAddTextures.end());
                for (int i = VERTEX_SIZE - 1; i < this->vertices.size(); i += VERTEX_SIZE)
                {
	                if (this->vertices[i] != 0)
	                {
                        this->vertices[i] = std::find(this->textures.begin(), this->textures.end(), textureCopy[this->vertices[i]]) - this->textures.begin();
	                }
                }

            }


	        for (int i = 0; i < renderData->textures.size(); i++) {
                std::vector<Shr<Texture>>::iterator textureIndex = std::find(this->textures.begin(), this->textures.end(), renderData->textures[i]);

                for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
                {
                    if (renderData->vertices[j] == i)
                    {
                        RDVerticesCopy[j] += textureIndex - this->textures.begin();
                    }
                }
			}
            renderData->oldTextures = renderData->textures;
        }


        if (renderData->vertices.size() == renderData->oldVertexSize)
        {
	        for (int i = 0; i < renderData->vertices.size(); i++)
	        {
                this->vertices[renderData->vertexSlot + i] = RDVerticesCopy.at(i);
	        }
        }


        //EBO
        if (renderData->ebo.size() == renderData->oldElementSize) {
            int batchVertexCount = renderData->vertexSlot / VERTEX_SIZE;
            for (int i = 0; i < renderData->ebo.size(); i++)
            {
                this->elements[renderData->elementSlot + i] = renderData->ebo.at(i) + batchVertexCount;
            }
        }

        reloadBufferArrays = true;
    }



    void RenderBatch::removeVertexProperties(RenderData* renderData) {
        if (std::find(this->dataBlocks.begin(), this->dataBlocks.end(), renderData) == this->dataBlocks.end()) return; //check if struct is in this batch

        if (renderData->textures.size() > 0)
        {
            std::vector<Shr<Texture>> toRemoveTextures;
            for (Shr<Texture> textureOld : renderData->oldTextures)
            {
                bool found = false;
                for (Shr<Texture> textureNew : renderData->textures)
                {
                    if (textureOld == textureNew)
                    {
                        found = true;
                    }
                }
                if (!found)
                {
                    toRemoveTextures.emplace_back(textureOld);
                }
            }

            //check if removed textures are still beeing used
            for (Shr<Texture> texture : toRemoveTextures)
            {
                for (RenderData* rd : this->dataBlocks)
                {
                    if (rd == renderData) continue;
                    std::vector<Shr<Texture>>::iterator it = std::find(toRemoveTextures.begin(), toRemoveTextures.end(), texture);
                    if (it != toRemoveTextures.end())
                    {
                        toRemoveTextures.erase(it);
                    }
                }
            }

            //remove texture
            std::vector<Shr<Texture>> textureCopy = this->textures;
            for (Shr<Texture> textureToRemove : toRemoveTextures)
            {
                std::vector<Shr<Texture>>::iterator it = std::find(this->textures.begin(), this->textures.end(), textureToRemove);
		        if (it != this->textures.end())
		        {
                    this->textures.erase(it);
		        }
            }
            for (int i = VERTEX_SIZE - 1; i < this->vertices.size(); i += VERTEX_SIZE)
            {
                if (this->vertices[i] != 0)
                {
                    this->vertices[i] = std::find(this->textures.begin(), this->textures.end(), textureCopy[this->vertices[i]]) - this->textures.begin();
                }
            }
        }

        for (int i = 0; i < renderData->vertices.size(); i++)
        {
            this->vertices.erase(this->vertices.begin() + renderData->vertexSlot);
        }
        for (int i = 0; i < renderData->ebo.size(); i++)
        {
            this->elements.erase(this->elements.begin() + renderData->elementSlot);
        }
        for (int i = renderData->dataBlockSlot; i < this->dataBlocks.size(); i++)
        {
            this->dataBlocks[i]->vertexSlot -= renderData->vertices.size();
            this->dataBlocks[i]->dataBlockSlot--;
        }

        dataBlocks.erase(dataBlocks.begin() + renderData->dataBlockSlot);
        reloadBufferArrays = true;
    }

    bool RenderBatch::HasTextureSpace(std::vector<Shr<Texture>> texs) {
        for (int i = 0; i < texs.size(); i++) {
            for (int j = 0; j < this->textures.size(); j++)
            {
                if (texs[i] == this->textures[j])
                {
                    texs.erase(texs.begin() + i);
                }
            }
        }
        return this->textures.size() + texs.size() <= 32;
    }

}
