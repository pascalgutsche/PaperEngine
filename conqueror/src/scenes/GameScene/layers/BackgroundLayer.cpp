#include "_Game.h"
#include "BackgroundLayer.h"

#include "event/KeyEvent.h"

void BackgroundLayer::OnAttach()
{
    background1 = new GameObject("background1", Transform(glm::vec2(-0.3f, 0.0f), glm::vec2(0.5f, 0.3f)), ProjectionMode::SCREEN);
    background2 = new GameObject("background2", Transform(glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));

    background1->AddComponent(new SpriteRenderer(glm::vec4(0.5f, 1.0f, 0.5f, 1.0f), Geometry::RECTANGLE));
    background2->AddComponent(new SpriteRenderer(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), Geometry::RECTANGLE));

    this->AddGameObjectToLayer(background1);
    this->AddGameObjectToLayer(background2);
}

void BackgroundLayer::OnDetach()
{
}

void BackgroundLayer::Update(const float dt)
{
    gameObjects[0]->transform.rotation += 100 * dt;
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
            auto vectora = GetGameObjectsByTag("a");
            auto vectorb = GetGameObjectsByTag("b");
            auto vectorc = GetGameObjectsByTag("c");

            for (auto go : vectora)
            {
                LOG_DEBUG("A: {0}", go->GetName());
            }
            for (auto go : vectorb)
            {
                LOG_DEBUG("B: {0}", go->GetName());
            }
            for (auto go : vectorc)
            {
                LOG_DEBUG("C: {0}", go->GetName());
            }
            return true;
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


