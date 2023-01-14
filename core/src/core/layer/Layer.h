#pragma once
#include "_Core.h"
#include "utility.h"

#include "event/ApplicationEvent.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"

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

		virtual bool OnKeyPressed(KeyPressedEvent& e) { return false; };
		virtual bool OnKeyReleased(KeyReleasedEvent& e) { return false; }
		virtual bool OnKeyTyped(KeyTypedEvent& e) { return false; }
		virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e) { return false; }
		virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) { return false; }
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) { return false; }
		virtual bool OnMouseMoved(MouseMovedEvent& e) { return false; }
		virtual bool OnWindowResize(WindowResizeEvent& e) { return false; }

		void event(Event& event);
		

		


		void setOverlayStatus(bool overlay) { this->overlay = overlay; }

		bool getOverlayStatus() { return overlay; }
	};
	
}

