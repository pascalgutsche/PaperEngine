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
    
    SpriteRenderer::SpriteRenderer(glm::vec4 color, Shr<Texture> texture)
        : color(color), sprite(new Sprite(nullptr)) { }

    SpriteRenderer::SpriteRenderer(glm::vec4 color, Sprite* sprite)
	    : color(color), sprite(sprite) { }

    SpriteRenderer::~SpriteRenderer() {
        delete sprite;
        delete lastTransform;
    }

    void SpriteRenderer::start() {
        // change values from the previous frame in order to check if the gameObject changed values
        // the local lastTransform variable always holds the old values, in order to be compared to the 'new' value
        lastTransform = gameObject->transform.copy();
    }

    void SpriteRenderer::stop()
    {
    }


    
    void SpriteRenderer::Update() {
        // check if there have been made changes to the sprite (transform of the gameObject)
        if (!(gameObject->transform.equals(*lastTransform)))
        {
            // if it is not equal, save it to the local transform and
            // set the dirty bit (variable that is being checked in order to display changes)
            gameObject->transform.copy(*this->lastTransform);
        }
        if (gameObject->GetProjectionMode() == ProjectionMode::PERSPECTIVE)
			Renderer::DrawRectangle(gameObject->transform.position, gameObject->transform.scale, color, gameObject->GetObjectID());
        else
    		Renderer::DrawTriangle(gameObject->transform.position, gameObject->transform.scale, color, gameObject->GetObjectID());


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
            }

            ImGui::Text("");
            ImGui::TreePop();
        }
        ImGui::BeginDisabled();
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
                std::shared_ptr<Texture> texture = DataPool::GetTexture(texturePaths[i]);

                const int IMGSIZE_HEIGHT = 100;
                Utils::Size ratio = Utils::calculateAspectRatioFit(texture->GetWidth(), texture->GetHeight(), texture->GetWidth() + IMGSIZE_HEIGHT, IMGSIZE_HEIGHT);

                ImGui::PushID(i);
                if (ImGui::ImageButton((void*)texture->GetID(), ImVec2(ratio.width, ratio.height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 2, ImColor(0, 0, 0, 1))) {
                    if (this->sprite->GetTexture() != texture);
                		//UpdateTexture(texture);
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
        ImGui::EndDisabled();
    }



    //void SpriteRenderer::UpdateTexture(Shr<Texture> texture)
    //{
    //    this->sprite->SetTexture(texture);
    //    renderData->textures.clear();
    //
    //    int textureInsert = renderData->textures.size();
    //    renderData->textures.emplace(renderData->textures.begin() + textureInsert, sprite->GetTexture());
    //
    //    for (int i = 8; i < renderData->vertices.size(); i += RenderBatch::GetVertexSize()) {
    //        renderData->vertices[i] = textureInsert;
    //    }
    //
    //    renderData->dirty = true;
    //}
}