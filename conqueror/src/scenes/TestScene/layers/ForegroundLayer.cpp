#include "_Game.h"
#include "ForegroundLayer.h"


void ForegroundLayer::OnAttach()
{
	character1 = new GameObject("character1", Transform(glm::vec2(3.0f, 3.0f), glm::vec2(2.0f, 2.0f)));
	character2 = new GameObject("character2", Transform(glm::vec2(0.0f, 3.0f), glm::vec2(2.0f, 2.0f)));
	object1 = new GameObject("object1", Transform(glm::vec2(-3, 3), glm::vec2(2.0f, 2.0f)));
	
	character1->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("james_webb.png"), SpriteRenderer::RECTANGLE));
	character2->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("placeholder_texture_256x256.png"), SpriteRenderer::RECTANGLE));
	object1->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("antimarx.png"), SpriteRenderer::RECTANGLE));
	
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
		character2->stop();
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