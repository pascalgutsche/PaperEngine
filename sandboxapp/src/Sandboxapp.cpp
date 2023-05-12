#include "_Game.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"

#include "scenes/SandboxScene/SandboxScene.h"

class Sandboxapp : public Application {
public:
	Sandboxapp()
		: Application(WindowProps("Sandbox", 1920, 1080)) { }
	
	~Sandboxapp() override {
		
	}

	void Init() override {
		// Uncomment and set the start scene (pointer)
		Application::ChangeScene(new SandboxScene()); 
	}

};

Application* core::CreateApplication() {
	return new Sandboxapp();
}
