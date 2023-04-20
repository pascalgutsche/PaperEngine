#include "_Game.h"
#include "GameScene.h"

#include "global.h"

GameScene::GameScene() {
    backcolor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    background_layer = new BackgroundLayer();
}

GameScene::~GameScene() {
    
}

void GameScene::OnStart() {
    AddLayer(background_layer);
}

void GameScene::OnStop()
{
	RemoveLayer(background_layer);
}


void GameScene::OnUpdate() {
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

bool GameScene::OnKeyPressed(KeyPressedEvent& e)
{
    if (e.getKeyCode() == KEY_C)
    {
        Application::ChangeScene(testScene);
    }
    return true;
}



