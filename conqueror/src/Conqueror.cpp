#define INCLUDE_MAIN
#include "Engine.h"

#include "scenes/TestScene/TestScene.h"

class Conqueror : public core::Application {
public:
	Conqueror() {
		
	}
	
	~Conqueror() {
		
	}

	void init() override {
		Application::changeScene(new TestScene());
	}

};

core::Application* core::createApplication() {
	return new Conqueror();
}