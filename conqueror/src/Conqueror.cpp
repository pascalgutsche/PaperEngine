#include "Engine.h"

class Conqueror : public core::Application {
public:
	Conqueror() {

	}
	
	~Conqueror() {

	}


};

core::Application* core::createApplication() {
	return new Conqueror();
}