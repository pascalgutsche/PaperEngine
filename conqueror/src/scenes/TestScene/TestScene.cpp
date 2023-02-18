#include "_Game.h"
#include "TestScene.h"
#include "layers/SoundLayer.h"


using namespace core;

TestScene::TestScene() {

}

TestScene::~TestScene() {
    Application::RemoveLayer(background_layer);
    Application::RemoveLayer(foreground_layer);
    Application::RemoveOverLay(sound_layer);
}

void TestScene::loadResources() {
    backcolor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    background_layer = new BackgroundLayer();
    foreground_layer = new ForegroundLayer();
    sound_layer = new SoundLayer();
}

void TestScene::init() {
    Application::AddLayer(background_layer);
    Application::AddLayer(foreground_layer);
    Application::AddOverLay(sound_layer);
}
int selectedItem = 0;

void TestScene::update(float dt) {

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

    if (Input::IsKeyPressed(KEY_LEFT)) {
        activeGameObject->transform.scale.x += 2.0f * dt;
    }
    if (Input::IsKeyPressed(KEY_RIGHT)) {
        activeGameObject->transform.scale.x -= 2.0f * dt;
    }
    if (Input::IsKeyPressed(KEY_UP)) {
        activeGameObject->transform.scale.y += 2.0f * dt;
    }
    if (Input::IsKeyPressed(KEY_DOWN)) {
        activeGameObject->transform.scale.y -= 2.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_U)) {
        Application::ChangeScene(new TestScene());
    }


    Render();
}

void TestScene::imgui(float dt) {
	
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



