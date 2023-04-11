#include "_Game.h"
#include "BackgroundLayer.h"

#include "event/KeyEvent.h"

void BackgroundLayer::OnAttach()
{
    background1 = new GameObject("background1", Transform(glm::vec2(-0.3f, 0.0f), glm::vec2(2.0f, 1.0f)));
    background2 = new GameObject("background2", Transform(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));

    background1->AddComponent(new SpriteRenderer(glm::vec4(0.5f, 1.0f, 0.5f, 0.7f), Geometry::RECTANGLE));
    background2->AddComponent(new SpriteRenderer(glm::vec4(0.0f, 0.0f, 1.0f, 0.3f), Geometry::RECTANGLE));

    //this->AddGameObjectToLayer(background2);
    //this->AddGameObjectToLayer(background1);

    panel = new Panel(glm::vec4(1.0f, 0.2f, 1.0f, 0.3f), Transform(glm::vec2(0.0f, 0.0f), glm::vec2(3.0f, 2.0f)), Type::Rectangle);
    Panel* panel1 = new Panel(glm::vec4(1.0f, 0.0f, 0.0f, 0.9f), Transform(glm::vec2(1.0f, 1.0f), glm::vec2(2.0f, 2.0f)), Type::Triangle);
    panel->AddChildObject(panel1);
    //Panel* panel2 = new Panel(glm::vec4(0.0f, 0.9f, 0.5f, 0.7f), Transform(glm::vec2(1.0f, -1.0f), glm::vec2(1.7783465f, 1.3f)), Type::Rectangle);
    //panel1->AddChildObject(panel2);
    AddUIObject(panel, ProjectionMode::PERSPECTIVE);
}

void BackgroundLayer::OnDetach()
{
}

void BackgroundLayer::Update(const float dt)
{
    //gameObjects[0]->transform.rotation += 100 * dt;
    
}

void BackgroundLayer::Imgui(const float dt)
{
}

void BackgroundLayer::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>([this](KeyPressedEvent& e)
    {
        if (e.getKeyCode() == KEY_C)
        {
        }
        if (e.getKeyCode() == KEY_M)
        {
            GameObject* gm = new GameObject("Background", Transform(glm::vec2(0.0f, 1.0f), glm::vec2(5.0f, 1.0f)));
            gm->AddComponent(new SpriteRenderer(glm::vec4(0.7f, 0.3f, 0.2f, 1.0f), Geometry::RECTANGLE));
            AddGameObjectToLayer(gm);
        }
        if (e.getKeyCode() == KEY_I)
        {
            this->Detach();
        }
        return false;
    });
}


