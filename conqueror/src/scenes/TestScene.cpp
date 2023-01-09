#include "TestScene.h"

using namespace core;


Sound soundSel;
Sound soundTroll;

TestScene::TestScene() {

}

TestScene::~TestScene() {

}

void TestScene::loadResources() {
    backcolor = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);
}

void TestScene::init() {
    GameObject* gameObject1 = new GameObject("Test", Transform(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
    GameObject* gameObject2 = new GameObject("Test", Transform(glm::vec2(1.0f, 2.0f), glm::vec2(1.0f, 1.0f)));
    GameObject* gameObject3 = new GameObject("Test", Transform(glm::vec2(0.0f, 4.0f), glm::vec2(1.0f, 1.0f)));
    GameObject* gameObject4 = new GameObject("Test", Transform(glm::vec2(0.0f, 6.0f), glm::vec2(1.0f, 1.0f)));

    gameObject1->addComponent(new SpriteRenderer(new Sprite(DataPool::getTexture("server-icon.png"))));
    gameObject2->addComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));
    gameObject3->addComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));
    gameObject4->addComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));

    //gameObject3->setZIndex(1);
    //gameObject4->setZIndex(1);

    this->addGameObjectToScene(gameObject1);
    this->addGameObjectToScene(gameObject2);
    this->addGameObjectToScene(gameObject3);
    this->addGameObjectToScene(gameObject4);


    //gameObject5->setZIndex(100);

    //this->activeGameObject = gameObject3;
    soundSel.loadSound("assets/sounds/menu.wav");
    soundTroll.loadSound("assets/sounds/allahu_akbar.mp3");
}
int selectedItem = 0;
float volume = 1.0f;

void TestScene::update(float deltaTime) {
    if (MouseListener::getScrollY() > 0) {
        camera->position.z += 10.0f * deltaTime;
    }
    else if (MouseListener::getScrollY() < 0) {
        camera->position.z -= 10.0f * deltaTime;
    }
    else if (MouseListener::getScrollX() > 0) {
        //camera.fov += 10.0f * deltaTime;
    }
    else if (MouseListener::getScrollX() < 0) {
        //camera.fov -= 10.0f * deltaTime;
    }

    if (KeyListener::isKeyPressed(KEY_G))
    {
        soundSel.playSound();
        volume = soundSel.getVolume();
    }

    if (KeyListener::isKeyPressed(KEY_Q))
    {
        soundTroll.playSound();
    }

    if (KeyListener::isKeyPressed(KEY_H))
    {
        soundSel.stopSound();
    }

    if (KeyListener::isKeyPressed(KEY_J))
    {
        volume += 0.1f;
        soundSel.setVolume(volume);
    }

    if (KeyListener::isKeyPressed(KEY_K))
    {
        volume -= 0.1f;
        soundSel.setVolume(volume);
    }

    if (KeyListener::isKeyPressed(KEY_S)) {
        camera->position.y -= 2.0f * deltaTime;
    }

    if (KeyListener::isKeyPressed(KEY_W)) {
        camera->position.y += 2.0f * deltaTime;
    }

    if (KeyListener::isKeyPressed(KEY_D)) {
        camera->position.x += 2.0f * deltaTime;
    }

    if (KeyListener::isKeyPressed(KEY_A)) {
        camera->position.x -= 2.0f * deltaTime;
    }

    if (KeyListener::isKeyPressed(KEY_LEFT)) {
        activeGameObject->transform.scale.x += 2.0f * deltaTime;
    }
    if (KeyListener::isKeyPressed(KEY_RIGHT)) {
        activeGameObject->transform.scale.x -= 2.0f * deltaTime;
    }
    if (KeyListener::isKeyPressed(KEY_UP)) {
        activeGameObject->transform.scale.y += 2.0f * deltaTime;
    }
    if (KeyListener::isKeyPressed(KEY_DOWN)) {
        activeGameObject->transform.scale.y -= 2.0f * deltaTime;
    }

    if (KeyListener::isKeyPressed(KEY_P)) {
        Application::get()->changeScene(new TestScene());
    }


    this->renderer->render(deltaTime);
}

void TestScene::imgui(float deltaTime) {
	
}
