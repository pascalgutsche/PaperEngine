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
        const BufferLayout layout = {
            { GLSLDataType::FLOAT2, "aPos" },
            { GLSLDataType::FLOAT4, "aColor" },
            { GLSLDataType::FLOAT2, "aTexCoord" },
            { GLSLDataType::FLOAT , "aTexID" }
        };

        vertexArray = VertexArray::CreateArray();
        vertexBuffer = VertexBuffer::CreateBuffer(layout, MAX_VERTEX_COUNT * VERTEX_SIZE_BYTES);
        elementBuffer = ElementBuffer::CreateBuffer(MAX_ELEMENT_COUNT * sizeof(unsigned int));

        vertexArray->SetVertexBuffer(vertexBuffer);
        vertexArray->SetElementBuffer(elementBuffer);
    }

    int RenderBatch::render() {
        for (RenderData* data : dataBlocks) {
            if (data->dirty) {
                updateVertexProperties(data);
                data->dirty = false;
            }
        }

        vertexArray->Bind();
        vertexBuffer->Bind();
        elementBuffer->Bind();

        vertexBuffer->BufferSubData();
        elementBuffer->BufferSubData();

        // use the shader and upload the shader variables
        shader->use();


        switch (displayMode)
        {
        case PERSPECTIVE:
            shader->uploadMat4f("uProjection", Application::GetActiveScene()->GetCamera()->getProjectionMatrix());
            break;
        case ORTHOGRAPHIC:
            shader->uploadMat4f("uProjection", Application::GetActiveScene()->GetCamera()->getOrthographicMatrix());
            break;
        case SCREEN:
            //shader->uploadVec2f("uProjection", glm::vec2(vertices[POS_OFFSET], vertices[POS_OFFSET + 1]));
            break;
        }
        shader->uploadMat4f("uView", Application::GetActiveScene()->GetCamera()->getViewMatrix());


        int* texArray = new int[32];
        for (int i = 0; i < textures.size(); i++)
        {
            // activate all created textures
            textures[i]->bind(i);
        }
        for (int i = 0; i < 32; i++)
        {
            texArray[i] = i;
        }
        // upload the texture array into the shader
        // convert vector to array in order to pass the parameters through
        shader->uploadIntArray("uTexture", textures.size(), texArray);
        

#ifdef BUILD_DEBUG
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
#endif


		glDrawElements(GL_TRIANGLES, elementBuffer->GetElementCount(), GL_UNSIGNED_INT, nullptr);
        draw_calls++;

        // unbind everything

        vertexBuffer->Unbind();
        elementBuffer->Unbind();
        vertexArray->Unbind();
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
        if (renderData->id != 0)
        {
            LOG_CORE_ERROR("Adding a render struct, which already has an ID! Aborting...");
            return;
        }
        renderData->id = Renderer::RequestID();

        //std::vector<float> RDVerticesCopy = renderData->vertices;
        ////TEXTURE
        //for (int i = 0; i < renderData->textures.size(); i++) {
        //    std::vector<Shr<Texture>>::iterator textureIndex = std::find(this->textures.begin(), this->textures.end(), renderData->textures[i]);
        //    if (textureIndex == this->textures.end()) {
        //        textureIndex = this->textures.insert(this->textures.end(), renderData->textures.begin(), renderData->textures.end());
        //    }
        //    
        //    for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
        //    {
        //        if (renderData->vertices[j] == i)
        //        {
        //            RDVerticesCopy[j] += textureIndex - this->textures.begin();
        //        }
        //    }
        //}
        //for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
        //{
        //    if (renderData->vertices[j] == -1)
        //    {
        //        RDVerticesCopy[j] = 32;
        //    }
        //}



        //VBO
        if (renderData->vertices.size() <= 0)
        {
            CORE_ASSERT(renderData->vertices.size() > 0, "invalid vbo buffer");
            return;
        }

        //EBO
        if (renderData->ebo.size() <= 0)
        {
            CORE_ASSERT(renderData->ebo.size() > 0, "invalid ebo buffer");
            return;
        } 

    	vertexBuffer->Add(renderData->vertices, renderData->id);
        elementBuffer->Add(renderData->ebo, renderData->id);

        
        renderData->oldTextures = renderData->textures;
        dataBlocks.emplace_back(renderData);
    }

    void RenderBatch::updateVertexProperties(RenderData* renderData)
    {
        if (renderData->id == 0)
        {
            LOG_CORE_ERROR("Updating a render struct, which has no ID! Aborting...");
            return;
        }

        //std::vector<float> RDVerticesCopy = renderData->vertices;
        ////TEXTURES
        //if (renderData->textures != renderData->oldTextures)
        //{
        //    //checking which texture was removed;
        //    std::vector<Shr<Texture>> toRemoveTextures;
        //    for (Shr<Texture> textureOld : renderData->oldTextures)
        //    {
        //        bool found = false;
	    //        for (Shr<Texture> textureNew : renderData->textures)
	    //        {
		//            if (textureOld == textureNew)
		//            {
        //                found = true;
		//            }
	    //        }
        //        if (!found)
        //        {
        //            toRemoveTextures.emplace_back(textureOld);
        //        }
        //    }
        //
        //    //check if removed textures are still beeing used
        //    for (int i = 0; i < toRemoveTextures.size(); i++)
        //    {
	    //        for (RenderData* rd : dataBlocks)
	    //        {
        //            if (rd == renderData) continue;
        //            if (std::find(rd->textures.begin(), rd->textures.end(), toRemoveTextures[i]) !=  rd->textures.end())
        //            {
        //                toRemoveTextures.erase(toRemoveTextures.begin() + i);
        //                break;
        //            }
	    //        }
        //    }
        //
        //    //check which texture is new
        //    std::vector<Shr<Texture>> toAddTextures;
        //    for (Shr<Texture> texture : renderData->textures)
        //    {
	    //        if (std::find(this->textures.begin(), this->textures.end(), texture) == this->textures.end())
	    //        {
        //            toAddTextures.emplace_back(texture);
	    //        }
        //    }
        //
        //    if (this->textures.size() + toAddTextures.size() - toRemoveTextures.size() > 32)
        //    {
        //        removeVertexProperties(renderData);
        //        Application::getCurrentScene()->GetRenderer().add(renderData);
        //    }
        //    else
        //    {
        //        std::vector<Shr<Texture>> textureCopy = this->textures;
        //        for (int i = 0; i < toRemoveTextures.size(); i++)
        //        {
        //            for (int j = 0; j < this->textures.size(); j++)
        //            {
        //                if (toRemoveTextures[i] == this->textures[j])
        //                {
        //                    this->textures.erase(this->textures.begin() + j);
        //                }
        //            }
        //        }
        //
        //        //add textures to renderbatch and change the structs to the right value
        //        this->textures.insert(this->textures.end(), toAddTextures.begin(), toAddTextures.end());
        //        for (int i = VERTEX_SIZE - 1; i < this->vertices.size(); i += VERTEX_SIZE)
        //        {
        //            if (this->vertices[i] == -1) this->vertices[i] = 32; // convert 'no texture' value to a positive value
	    //            if (this->vertices[i] != 32)
	    //            {
        //                this->vertices[i] = std::find(this->textures.begin(), this->textures.end(), textureCopy[this->vertices[i]]) - this->textures.begin();
	    //            }
        //        }
        //
        //    }
        //
        //
	    //    for (int i = 0; i < renderData->textures.size(); i++) {
        //        std::vector<Shr<Texture>>::iterator textureIndex = std::find(this->textures.begin(), this->textures.end(), renderData->textures[i]);
        //
        //        for (int j = VERTEX_SIZE - 1; j < renderData->vertices.size(); j += VERTEX_SIZE)
        //        {
        //            if (renderData->vertices[j] == i)
        //            {
        //                RDVerticesCopy[j] += textureIndex - this->textures.begin();
        //            }
        //        }
		//	}
        //    renderData->oldTextures = renderData->textures;
        //}

		vertexBuffer->Update(renderData->vertices, renderData->id);
        elementBuffer->Update(renderData->ebo, renderData->id);
    }



    void RenderBatch::removeVertexProperties(RenderData* renderData)
	{
        if (renderData->id == 0)
        {
            LOG_CORE_ERROR("Removing a render struct, which has no ID! Aborting...");
            return;
        }

        //if (std::find(this->dataBlocks.begin(), this->dataBlocks.end(), renderData) == this->dataBlocks.end()) return; //check if struct is in this batch
        //if (renderData->textures.size() > 0)
        //{
        //    std::vector<Shr<Texture>> toRemoveTextures;
        //    for (Shr<Texture> textureOld : renderData->oldTextures)
        //    {
        //        bool found = false;
        //        for (Shr<Texture> textureNew : renderData->textures)
        //        {
        //            if (textureOld == textureNew)
        //            {
        //                found = true;
        //            }
        //        }
        //        if (!found)
        //        {
        //            toRemoveTextures.emplace_back(textureOld);
        //        }
        //    }
        //
        //    //check if removed textures are still beeing used
        //    for (Shr<Texture> texture : toRemoveTextures)
        //    {
        //        for (RenderData* rd : this->dataBlocks)
        //        {
        //            if (rd == renderData) continue;
        //            std::vector<Shr<Texture>>::iterator it = std::find(toRemoveTextures.begin(), toRemoveTextures.end(), texture);
        //            if (it != toRemoveTextures.end())
        //            {
        //                toRemoveTextures.erase(it);
        //            }
        //        }
        //    }
        //
        //    //remove texture
        //    std::vector<Shr<Texture>> textureCopy = this->textures;
        //    for (Shr<Texture> textureToRemove : toRemoveTextures)
        //    {
        //        std::vector<Shr<Texture>>::iterator it = std::find(this->textures.begin(), this->textures.end(), textureToRemove);
		//        if (it != this->textures.end())
		//        {
        //            this->textures.erase(it);
		//        }
        //    }
        //    for (int i = VERTEX_SIZE - 1; i < this->vertices.size(); i += VERTEX_SIZE)
        //    {
        //        if (this->vertices[i] == -1) this->vertices[i] = 32;
        //        if (this->vertices[i] != 32)
        //        {
        //            this->vertices[i] = std::find(this->textures.begin(), this->textures.end(), textureCopy[this->vertices[i]]) - this->textures.begin();
        //        }
        //    }
        //}

        vertexBuffer->Remove(renderData->id);
        elementBuffer->Remove(renderData->id);
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
