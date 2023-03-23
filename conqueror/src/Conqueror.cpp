#include "_Game.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"

#include "scenes/TestScene/TestScene.h"

class Conqueror : public Application {
public:
	Conqueror() {
		
	}
	
	~Conqueror() override {
		
	}

	void Init() override {
		Application::ChangeScene(new TestScene());
	}

};

Application* core::CreateApplication() {
	return new Conqueror();
}