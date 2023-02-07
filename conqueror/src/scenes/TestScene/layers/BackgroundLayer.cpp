#include "_Game.h"
#include "BackgroundLayer.h"

void BackgroundLayer::OnAttach()
{
    background1 = new GameObject("background1", Transform(glm::vec2(2.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    background2 = new GameObject("background2", Transform(glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    background3 = new GameObject("background3", Transform(glm::vec2(-2.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    background4 = new GameObject("background4", Transform(glm::vec2(2.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    background5 = new GameObject("background5", Transform(glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    background6 = new GameObject("background6", Transform(glm::vec2(-2.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    //background7 = new GameObject("background7", Transform(glm::vec2(-1.0f, -1.0f), glm::vec2(0.2f, 0.2f)), SCREEN);
    //background8 = new GameObject("background8", Transform(glm::vec2(3.0f, -4.0f), glm::vec2(2.0f, 2.0f)));

    background1->addComponent(new SpriteRenderer(glm::vec4(0.5f, 1.0f, 0.5f, 1.0f)));
    background2->addComponent(new SpriteRenderer(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
    background3->addComponent(new SpriteRenderer(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
    background4->addComponent(new SpriteRenderer(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)));
    background5->addComponent(new SpriteRenderer(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
    background6->addComponent(new SpriteRenderer(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
    //background7->addComponent(new SpriteRenderer(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), DataPool::getTexture("james_webb.png")));
    //background8->addComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));


    this->AddGameObjectToLayer(background1);
    this->AddGameObjectToLayer(background2);
    this->AddGameObjectToLayer(background3);
    this->AddGameObjectToLayer(background4);
    this->AddGameObjectToLayer(background5);
    this->AddGameObjectToLayer(background6);
    //this->AddGameObjectToLayer(background7);
    //this->AddGameObjectToLayer(background8);
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
}

void BackgroundLayer::imgui(const float dt)
{
}

void BackgroundLayer::OnEvent(Event& event)
{
}
