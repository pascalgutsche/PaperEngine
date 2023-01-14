#include "_Core.h"

#include "Layer.h"

namespace core
{
	void Layer::event(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Layer::OnKeyPressed));
		dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Layer::OnKeyReleased));
		dispatcher.dispatch<KeyTypedEvent>(BIND_EVENT_FN(Layer::OnKeyTyped));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(Layer::OnMouseButtonPressed));
		dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(Layer::OnMouseButtonReleased));
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Layer::OnMouseScrolled));
		dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(Layer::OnMouseMoved));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Layer::OnWindowResize));


	};
}
