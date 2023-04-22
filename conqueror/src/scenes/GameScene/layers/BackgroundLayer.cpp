#include "_Game.h"
#include "BackgroundLayer.h"

#include "event/KeyEvent.h"

void BackgroundLayer::OnAttach()
{
    background1 = new GameObject("background1", Transform());
    background2 = new GameObject("background2", Transform(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));

    background1->AddComponent(new SpriteRenderer(glm::vec4(0.5f, 1.0f, 0.5f, 0.7f), Geometry::RECTANGLE));
    background2->AddComponent(new SpriteRenderer(glm::vec4(0.0f, 0.0f, 1.0f, 0.3f), Geometry::RECTANGLE));

    this->AddGameObjectToLayer(background1);
    //this->AddGameObjectToLayer(background2);

    GameObject* text = new GameObject("text");
    text->AddComponent(new FontRenderer(glm::vec4(1.0f), "Dg"));
    AddGameObjectToLayer(text);

    //Label* label = new Label("cc", glm::vec4(1), Transform(glm::vec2(0.0f)), DataPool::GetFont("mononoki.ttf"));
    //AddUIObject(label, ProjectionMode::PERSPECTIVE);

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


