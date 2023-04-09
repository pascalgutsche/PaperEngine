#include "_Game.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"
#include "scenes/GameScene/GameScene.h"

#include "scenes/TestScene/TestScene.h"

class Conqueror : public Application {
public:
	Conqueror() {
		
	}
	
	~Conqueror() override {
		
	}

	void Init() override {
		Application::ChangeScene(new GameScene());
	}

};

Application* core::CreateApplication() {
	return new Conqueror();
}