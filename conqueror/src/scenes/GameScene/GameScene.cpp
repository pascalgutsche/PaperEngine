#include "_Game.h"
#include "GameScene.h"

#include "scenes/TestScene/TestScene.h"

using namespace core;

GameScene::GameScene() {

}

GameScene::~GameScene() {
    RemoveLayer(background_layer);
}

void GameScene::LoadResources() {
    backcolor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    background_layer = new BackgroundLayer();
}

void GameScene::Init() {
    AddLayer(background_layer);
}

void GameScene::Update() {
    float dt = Application::GetDT();
    if (Input::IsKeyPressed(KEY_S)) {
        camera->position.y -= 5.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_W)) {
        camera->position.y += 5.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_D)) {
        camera->position.x += 5.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_A)) {
        camera->position.x -= 5.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_U)) {
        Application::ChangeScene(new TestScene());
    }
}

void GameScene::Imgui(float dt) {
	
}

bool GameScene::OnMouseScroll(MouseScrolledEvent& e)
{
    if (this->GetCamera()->position.z > 0.2f && e.GetYOffset() != 0)
        this->GetCamera()->position.z += 10.0 * core::Application::GetDT() * e.GetYOffset();
    if (this->GetCamera()->position.z < 0.2f) {
        this->GetCamera()->position.z = 0.2f;
    }
        
    return true;
}



