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
    backcolor = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);
    background_layer = new BackgroundLayer();
    foreground_layer = new ForegroundLayer();
    sound_layer = new SoundLayer();
}

void TestScene::init() {
    Application::AddLayer(background_layer);
    Application::AddLayer(foreground_layer);
    Application::AddOverLay(sound_layer);

    //gameObject5->setZIndex(100);

    //this->activeGameObject = gameObject3;
    
}
int selectedItem = 0;

void TestScene::update(float dt) {

    if (Input::IsKeyPressed(KEY_S)) {
        camera->position.y -= 2.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_W)) {
        camera->position.y += 2.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_D)) {
        camera->position.x += 2.0f * dt;
    }

    if (Input::IsKeyPressed(KEY_A)) {
        camera->position.x -= 2.0f * dt;
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


    this->renderer->render(dt);
}

void TestScene::imgui(float dt) {
	
}

bool TestScene::OnMouseScroll(MouseScrolledEvent& e)
{
    if (this->getCamera()->position.z > 1)
        this->getCamera()->position.z += 10.0f * core::Application::GetDT() * e.getYOffset();
    else if (e.getYOffset() > 0)
        this->getCamera()->position.z += 10.0f * core::Application::GetDT() * e.getYOffset();
    this->getCamera()->fov += 1.0f * core::Application::GetDT() * e.getXOffset();
    return true;
}



