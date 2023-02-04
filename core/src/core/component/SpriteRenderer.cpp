#include "_Core.h"

#include "component/SpriteRenderer.h"
#include "generic/GameObject.h"
#include "renderer/Texture.h"
#include "utils/DataPool.h"
#include "utils/Utils.h"
#include "renderer/RenderBatch.h"

#include "imgui/ImGuiLayer.h"

#include "generic/Application.h"

namespace core {

    //SPRITERENDERER
    SpriteRenderer::SpriteRenderer(glm::vec4 color, Shr<Texture> texture)
        : Component("sprite_renderer")
	{
        this->color = color;
        this->sprite = new Sprite(texture);
    }

    SpriteRenderer::SpriteRenderer(glm::vec4 color, Sprite* sprite)
	    : Component("sprite_renderer")
    {
        this->color = color;
        this->sprite = sprite;
    }


    SpriteRenderer::~SpriteRenderer() {
        delete sprite;
        delete lastTransform;
        delete renderData;
    }

    void SpriteRenderer::start() {
        // change values from the previous frame in order to check if the gameObject changed values
        // the local lastTransform variable always holds the old values, in order to be compared to the 'new' value
        lastTransform = core::GameObject::CGMap[this]->transform.copy();
        
        renderData = new RenderData();

        renderData->displayMode = GameObject::CGMap[this]->displayMode;
        renderData->zIndex = GameObject::CGMap[this]->getZIndex();

        int textureInsert = -1;

        if (sprite->getTexture() != nullptr) {
            textureInsert = renderData->textures.size();
            renderData->textures.emplace(renderData->textures.begin() + textureInsert, sprite->getTexture());
        }
        // please have mercy with us

        float xAdd = 0.0f;
        float yAdd = 0.0f;
        for (int i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 1:
                xAdd = 1.0f;
                break;
            case 2:
                yAdd = 1.0f;
                break;
            case 3:
                xAdd = 0.0f;
                break;
            default:
                break;
            }

        	renderData->vertices.emplace(renderData->vertices.end(), GameObject::CGMap[this]->transform.position.x + xAdd * GameObject::CGMap[this]->transform.scale.x);
            renderData->vertices.emplace(renderData->vertices.end(), GameObject::CGMap[this]->transform.position.y + yAdd * GameObject::CGMap[this]->transform.scale.y);

            renderData->vertices.emplace(renderData->vertices.end(), this->color.x);
            renderData->vertices.emplace(renderData->vertices.end(), this->color.y);
            renderData->vertices.emplace(renderData->vertices.end(), this->color.z);
            renderData->vertices.emplace(renderData->vertices.end(), this->color.w);

            renderData->vertices.emplace(renderData->vertices.end(), xAdd); // TODO: insert texture coord from sprite here
            renderData->vertices.emplace(renderData->vertices.end(), yAdd);

            renderData->vertices.emplace(renderData->vertices.end(), textureInsert);
        }

        renderData->ebo.emplace(renderData->ebo.end(), 0);
        renderData->ebo.emplace(renderData->ebo.end(), 1);
        renderData->ebo.emplace(renderData->ebo.end(), 2);
        renderData->ebo.emplace(renderData->ebo.end(), 2);
        renderData->ebo.emplace(renderData->ebo.end(), 3);
        renderData->ebo.emplace(renderData->ebo.end(), 0);

        Application::getCurrentScene()->GetRenderer().add(renderData);
    }

    void SpriteRenderer::stop()
    {
        Application::getCurrentScene()->GetRenderer().remove(renderData);
    }


    
    void SpriteRenderer::update(float dt) {
        // check if there have been made changes to the sprite (transform of the gameObject)
        if (!(GameObject::CGMap[this]->transform.equals(*lastTransform)))
        {
            // if it is not equal, save it to the local transform and
            // set the dirty bit (variable that is being checked in order to display changes)
            GameObject::CGMap[this]->transform.copy(*this->lastTransform);

            //update position in struct
            float xAdd = 0.0f;
            float yAdd = 0.0f;
            for (int i = 0; i < 4; i ++)
            {
                switch (i)
                {
                case 1:
                    xAdd = 1.0f;
                    break;
                case 2:
                    yAdd = 1.0f;
                    break;
                case 3:
                    xAdd = 0.0f;
                    break;
                default:
                    break;
                }
                renderData->vertices[i * RenderBatch::GetVertexSize()] = GameObject::CGMap[this]->transform.position.x + xAdd * GameObject::CGMap[this]->transform.scale.x;
                renderData->vertices[(i * RenderBatch::GetVertexSize()) + 1] = GameObject::CGMap[this]->transform.position.y + yAdd * GameObject::CGMap[this]->transform.scale.y;
            }
            renderData->dirty = true;
        }
    }

    static float timeUntilRefresh = 0.0f;
    static std::vector<std::string> texturePaths;

    void SpriteRenderer::imgui(float dt) {
        ImGui::Text("Component - SpriteRenderer:");
        if (ImGui::TreeNode("Color:"))
        {
            float colorArray[4]{
                    color.x,
                    color.y,
                    color.z,
                    color.w
            };
            ImGui::ColorPicker3("ColorPicker", colorArray, 0);
            if (!(color.x == colorArray[0] && color.y == colorArray[1] && color.z == colorArray[2] && color.w == colorArray[3])) {
                color = glm::vec4(colorArray[0], colorArray[1], colorArray[2], colorArray[3]);
                UpdateColor(color);
            }

            ImGui::Text("");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Texture:"))
        {
            timeUntilRefresh -= dt;
            if (timeUntilRefresh <= 0.0f) {
                texturePaths.erase(texturePaths.begin(), texturePaths.end());
                DIR* dir = opendir("assets/textures");
                struct dirent* entity;
                entity = readdir(dir);
                while (entity != NULL) {
                    std::string name = entity->d_name;
                    if (name != "." && name != "..") {
                        texturePaths.insert(texturePaths.end(), entity->d_name);
                    }
                    entity = readdir(dir);
                }
                timeUntilRefresh = 1.0f;
            }
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            float windowX2 = windowPos.x + windowSize.x;
            for (int i = 0; i < texturePaths.size(); i++) {
                std::shared_ptr<Texture> texture = DataPool::getTexture(texturePaths[i]);

                const int IMGSIZE_HEIGHT = 100;
                Utils::XY ratio = Utils::calculateAspectRatioFit(texture->getWidth(), texture->getHeight(), texture->getWidth() + IMGSIZE_HEIGHT, IMGSIZE_HEIGHT);

                ImGui::PushID(i);
                if (ImGui::ImageButton((void*)texture->getID(), ImVec2(ratio.width, ratio.height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 2, ImColor(0, 0, 0, 1))) {
                	if (this->sprite->getTexture() != texture)
                		UpdateTexture(texture);
                }
                ImGui::PopID();

                ImVec2 lastButtonPos = ImGui::GetItemRectMax();
                float lastButtonPosX2 = lastButtonPos.x;
                float nextButtonX2 = lastButtonPosX2 + ratio.width + 50.0f;
                if (i + 1 < texturePaths.size() && nextButtonX2 < windowX2) {
                    ImGui::SameLine();
                }
            }

            ImGui::Text("");
            ImGui::TreePop();
        }
    }

    

    void SpriteRenderer::UpdateColor(glm::vec4 localColor) {
	    const float colorBuffer[4]
        {
            localColor.x,
            localColor.y,
            localColor.z,
            localColor.w
        };

        for (int i = 2; i < renderData->vertices.size(); i+= RenderBatch::GetVertexSize()) {
            for (int j = 0; j < 4; j++) {
                renderData->vertices[i + j] = colorBuffer[j];
            }
        }

        renderData->dirty = true;
    }

    void SpriteRenderer::UpdateTexture(Shr<Texture> texture)
    {
        this->sprite->setTexture(texture);
        renderData->textures.clear();

        int textureInsert = renderData->textures.size();
        renderData->textures.emplace(renderData->textures.begin() + textureInsert, sprite->getTexture());

        for (int i = 8; i < renderData->vertices.size(); i += RenderBatch::GetVertexSize()) {
            renderData->vertices[i] = textureInsert;
        }

        renderData->dirty = true;
    }
}