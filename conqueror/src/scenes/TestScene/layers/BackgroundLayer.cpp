#include "_Game.h"
#include "BackgroundLayer.h"

void BackgroundLayer::OnAttach()
{
    background1 = new GameObject("background1", Transform(glm::vec2(2.0f, 1.0f), glm::vec2(1.0f, 1.0f)), ProjectionMode::SCREEN);
    background2 = new GameObject("background2", Transform(glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    background3 = new GameObject("background3", Transform(glm::vec2(-2.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    background4 = new GameObject("background4", Transform(glm::vec2(2.0f, -1.0f), glm::vec2(3.7f, 4.0f)), ProjectionMode::ORTHOGRAPHIC);
    background5 = new GameObject("background5", Transform(glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    background6 = new GameObject("background6", Transform(glm::vec2(-2.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    background7 = new GameObject("background7", Transform(glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.4f)), ProjectionMode::SCREEN);
    background8 = new GameObject("background8", Transform(glm::vec2(1.0f, 0.5f), glm::vec2(2.0f, 1.0f), -50.0f));

    background1->AddComponent(new SpriteRenderer(glm::vec4(0.5f, 1.0f, 0.5f, 1.0f), SpriteRenderer::RECTANGLE));
    background2->AddComponent(new SpriteRenderer(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), SpriteRenderer::RECTANGLE));
    background3->AddComponent(new SpriteRenderer(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), SpriteRenderer::TRIANGLE));
    background4->AddComponent(new SpriteRenderer(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), SpriteRenderer::RECTANGLE));
    background5->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), SpriteRenderer::RECTANGLE));
    background6->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), SpriteRenderer::RECTANGLE));
    background7->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), SpriteRenderer::RECTANGLE));
    background8->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), SpriteRenderer::RECTANGLE));


    this->AddGameObjectToLayer(background1);
    this->AddGameObjectToLayer(background2);
    this->AddGameObjectToLayer(background3);
    this->AddGameObjectToLayer(background4);
    this->AddGameObjectToLayer(background5);
    this->AddGameObjectToLayer(background6);
    this->AddGameObjectToLayer(background7);
    this->AddGameObjectToLayer(background8);
}

void BackgroundLayer::OnDetach()
{
    for (auto i : game_objects)
    {
        delete i;
    }
    game_objects.clear();
}

void BackgroundLayer::update(const float dt)
{
    game_objects[0]->transform.rotation += 100 * dt;
}

void BackgroundLayer::imgui(const float dt)
{
}

void BackgroundLayer::OnEvent(Event& event)
{
}
