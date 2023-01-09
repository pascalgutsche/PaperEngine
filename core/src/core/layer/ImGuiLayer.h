#pragma once
#include "_Core.h"

#include "layer/Layer.h"

#include "event/ApplicationEvent.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"

namespace core {

    class ImGuiLayer : public Layer {
    private:
        bool onMouseButtonPressed(MouseButtonPressedEvent& e);
        bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
        bool onMouseScrolled(MouseScrolledEvent& e);
        bool onMouseMoved(MouseMovedEvent& e);
        bool onKeyPressed(KeyPressedEvent& e);
        bool onKeyReleased(KeyReleasedEvent& e);
        bool onKeyTyped(KeyTypedEvent& e);
        bool onWindowResize(WindowResizeEvent& e);
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void attach() override;
        void detach() override;
        void update(const float dt) override;
        void event(Event& event) override;
        
        void begin(const float dt);
        void end();
    };

}