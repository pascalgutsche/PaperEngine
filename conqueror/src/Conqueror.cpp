#include "_Game.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"

#include "scenes/TestScene/TestScene.h"

class Conqueror : public core::Application {
public:
	Conqueror() {
		
	}
	
	~Conqueror() {
		
	}

	void init() override {
		Application::ChangeScene(new TestScene());
	}

};

core::Application* core::createApplication() {
	return new Conqueror();
}