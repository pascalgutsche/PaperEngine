#include "Editor.h"
#include "PELayer.h"

static ViewPort& GetActiveViewPort(std::vector<ViewPort>& viewports)
{


}

void PELayer::MousePicking()
{
    hovered_entity = Entity();
    ViewPort* port = nullptr;
    for (auto& viewport : viewports)
        if (viewport.is_visible && viewport.viewport_hovered)
            port = &viewport;
    if (!port) return;
    const ViewPort& viewPort = *port;

    auto [mx, my] = ImGui::GetMousePos();
    mx -= viewPort.viewport_bounds[0].x;
    my -= viewPort.viewport_bounds[0].y;
    glm::vec2 viewportSize = viewPort.viewport_bounds[1] - viewPort.viewport_bounds[0];
    my = viewportSize.y - my;

    glm::ivec2 mouse_pos{(int)mx, (int)my};

    if (!(mouse_pos.x >= 0 && mouse_pos.y >= 0 && mouse_pos.x < (int)viewportSize.x && mouse_pos.y < (int)viewportSize.y)) return; //mouse not inside any viewport

    viewPort.framebuffer->Bind();
    int pixelID = viewPort.framebuffer->ReadPixel(1, mouse_pos);
    viewPort.framebuffer->Unbind();

    pixelID > -1 ? hovered_entity = Entity((entt::entity)pixelID, scene.get()) : Entity();

    bool left_mouse_button_pressed = Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    static bool left_mouse_button_pressed_last_frame;

    if (hovered_entity && left_mouse_button_pressed && !left_mouse_button_pressed_last_frame) {
        active_entity = hovered_entity;
    }
    else if (!hovered_entity && left_mouse_button_pressed && !left_mouse_button_pressed_last_frame)
    {
        active_entity = Entity();
    }

    left_mouse_button_pressed_last_frame = left_mouse_button_pressed;

#if 0

    if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        mouseClickedID[0] = mouseHoverID[0];
        if (mouseClickedID[0] != mouseClickedID[1] && mouseClickedID[0] != -1 && !pressed)
        {
            Entity* gameObject = dynamic_cast<Entity*>(Core::GetObjectByID(mouseClickedID[0]));
            if (gameObject)
                Application::QueueEvents(new EntityPressedEvent(gameObject));
    
            Button* button = dynamic_cast<Button*>(Core::GetObjectByID(mouseClickedID[0]));
            if (button)
                button->buttonEventFunction();
        }
        pressed = true;
        mouseClickedID[1] = mouseClickedID[0];
    }
    else {
        pressed = false;
        if (mouseClickedID[1] != 0) {
            Entity* gameObject = dynamic_cast<Entity*>(Core::GetObjectByID(mouseClickedID[1]));
            if (gameObject)
                Application::QueueEvents(new EntityReleasedEvent(gameObject));
        }
    
    
        mouseClickedID[1] = 0;
        if (mouseHoverID[0] != mouseHoverID[1])
        {
            if (mouseHoverID[1] != 0) {
                Entity* gameObject = dynamic_cast<Entity*>(Core::GetObjectByID(mouseHoverID[1]));
                if (gameObject)
                    Application::QueueEvents(new EntityHoverEndEvent(gameObject));
            }
            if (mouseHoverID[0] != 0) {
                Entity* gameObject = dynamic_cast<Entity*>(Core::GetObjectByID(mouseHoverID[0]));
                if (gameObject)
                    Application::QueueEvents(new EntityHoverBeginEvent(gameObject));
            }
        }
        mouseHoverID[1] = mouseHoverID[0];
    }
#endif
}