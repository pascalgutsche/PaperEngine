#include "TestScene.h"

using namespace core;

bool con = true;

TestScene::TestScene() {

}

TestScene::~TestScene() {

}

void TestScene::loadResources() {
	
}

void TestScene::init() {

}

void TestScene::update(float deltaTime) {
	//this->renderer->render();
	if (con) {
		Application::get()->changeScene(new TestScene());
		con = false;
	}

	if (KeyListener::isKeyPressed(KEY_SPACE)) {
		
	}
	std::cout << "bunker" << std::endl;
}

void TestScene::imgui(float deltaTime) {
	
}
