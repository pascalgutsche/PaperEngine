#pragma once
#include "_Core.h"
#include "utility.h"

#include "event/Event.h"

namespace core
{

	class Layer
	{
	protected:
		std::string name;
		bool overlay = false;
	public:
		Layer(const std::string& name = "Layer")
			: name(name) { }
		virtual ~Layer() = default;

		virtual void attach() = 0;
		virtual void detach() = 0;
		virtual void update(const float dt) = 0;
		virtual void imgui(const float dt) = 0;
		virtual void event(Event& event) = 0;

		void setOverlayStatus(bool overlay) { this->overlay = overlay; }

		bool getOverlayStatus() { return overlay; }
	};
	
}

