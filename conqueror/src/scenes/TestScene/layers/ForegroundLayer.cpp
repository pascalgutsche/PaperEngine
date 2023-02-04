#include "ForegroundLayer.h"


void ForegroundLayer::OnAttach()
{
	character1 = new GameObject("character1", Transform(glm::vec2(-10.0f, 4.0f), glm::vec2(40.0f, 5.0f)));
	character2 = new GameObject("character2");
	object1 = new GameObject("object1", Transform(glm::vec2(-3, -10), glm::vec2(10.0f, 3.0f)));
	
	character1->addComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), DataPool::getTexture("IMG_0131.jpg")));
	character2->addComponent(new SpriteRenderer(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), DataPool::getTexture("antimarx.png")));
	object1->addComponent(new SpriteRenderer(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), DataPool::getTexture("Honecker-2113738823.jpg")));
	
	this->AddGameObjectToLayer(character1);
	this->AddGameObjectToLayer(character2);
	this->AddGameObjectToLayer(object1);
}

void ForegroundLayer::OnDetach()
{
	for (auto i : game_objects)
	{
		delete i;
	}
	game_objects.clear();
}

void ForegroundLayer::update(const float dt)
{
	static bool con = true;
	if (Input::IsKeyPressed(KEY_Y) && con)
	{
		con = false;
		character2->getComponent("sprite_renderer")->stop();
	}
	if (Input::IsKeyPressed(KEY_T) && !con)
	{
		con = true;
	}
}

void ForegroundLayer::imgui(const float dt)
{
}

void ForegroundLayer::OnEvent(Event& event)
{
}
