#include "_Game.h"
#include "TestScene.h"
#include "layers/SoundLayer.h"

#include "global.h"

using namespace core;

TestScene::TestScene() {

}

TestScene::~TestScene() {
    RemoveLayer(foreground_layer);
    RemoveOverlay(sound_layer);
}

void TestScene::LoadResources() {
    backcolor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    foreground_layer = new ForegroundLayer();
    sound_layer = new SoundLayer();
}

void TestScene::Init() {
    AddLayer(foreground_layer);
    AddOverlay(sound_layer);
}

void TestScene::Update() {
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
        Application::ChangeScene(new GameScene());
    }
}

void TestScene::Imgui(float dt) {
	
}

void TestScene::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(TestScene::OnMouseScroll));
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(TestScene::OnKeyPressed));
}

bool TestScene::OnMouseScroll(MouseScrolledEvent& e)
{
    if (this->GetCamera()->position.z > 0.2f && e.GetYOffset() != 0)
        this->GetCamera()->position.z += 10.0 * core::Application::GetDT() * e.GetYOffset();
    if (this->GetCamera()->position.z < 0.2f) {
        this->GetCamera()->position.z = 0.2f;
    }
        
    return true;
}

bool TestScene::OnKeyPressed(KeyPressedEvent& e)
{
    if (e.getKeyCode() == KEY_C)
    {
        Application::ChangeScene(gameScene);
    }
    return true;
}



