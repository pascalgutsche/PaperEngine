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
    Panel* panel2 = new Panel(glm::vec4(1.0f, 0.0f, 0.0f, 0.9f), Transform(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f)), Type::Triangle);
    panel->AddChildObject(panel2);
    Button* button = new Button(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f), Transform(glm::vec2(0.0f), glm::vec2(1.5f)), Type::Rectangle, BIND_BUTTON_EVENT_FN(BackgroundLayer::ButtonPressed));
    panel2->AddChildObject(button);
	Panel* buttonPanel = new Panel(glm::vec4(0.3f, 0.7f, 0.9f, 0.9f), Transform(glm::vec2(0.0f, -0.5f), glm::vec2(1.0f, 1.0f)), Type::Rectangle);
    button->AddChildObject(buttonPanel);
    Panel* panel3 = new Panel(glm::vec4(1.0f, 0.0f, 0.0f, 0.9f), Transform(glm::vec2(0.0f, 0.9f), glm::vec2(0.5f, 0.5f)), Type::Triangle);
    buttonPanel->AddChildObject(panel3);
    AddUIObject(panel, ProjectionMode::PERSPECTIVE);


    panel4 = new Panel(glm::vec4(1.0f, 0.2f, 1.0f, 0.3f), Transform(glm::vec2(6.0f, 0.0f), glm::vec2(3.0f, 2.0f)), Type::Rectangle);
    Panel* panel24 = new Panel(glm::vec4(1.0f, 0.0f, 0.0f, 0.9f), Transform(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f)), Type::Triangle);
    panel4->AddChildObject(panel24);
    Button* button4 = new Button(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f), Transform(glm::vec2(0.0f), glm::vec2(1.5f)), Type::Rectangle, BIND_BUTTON_EVENT_FN(BackgroundLayer::ButtonPressed4));
    panel24->AddChildObject(button4);
    Panel* buttonPanel4 = new Panel(glm::vec4(0.3f, 0.7f, 0.9f, 0.9f), Transform(glm::vec2(0.0f, -0.5f), glm::vec2(1.0f, 1.0f)), Type::Rectangle);
    button4->AddChildObject(buttonPanel4);
    Label* panel34 = new Label("bonkkkkkkk",glm::vec4(1.0f, 0.0f, 0.0f, 0.9f), Transform(glm::vec2(0.0f, 0.9f), glm::vec2(0.5f, 0.5f)));
    buttonPanel4->AddChildObject(panel34);
    AddUIObject(panel4, ProjectionMode::PERSPECTIVE);
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

void BackgroundLayer::ButtonPressed()
{
    LOG_CORE_ERROR("PRESSED");
}

void BackgroundLayer::ButtonPressed4()
{
    LOG_CORE_ERROR("OMG");
}


