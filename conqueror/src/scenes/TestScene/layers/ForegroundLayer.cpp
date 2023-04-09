#include "_Game.h"
#include "ForegroundLayer.h"

#include "component/CircleRenderer.h"
#include "component/SpriteSheet.h"
#include "component/FontRenderer.h"

void ForegroundLayer::OnAttach()
{
	font = new GameObject("fontBomb", Transform(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 0.5f), 45.0f));
	spriteObject1 = new GameObject("spritePain", Transform(glm::vec2(0.436f, 3.32432f), glm::vec2(1.5, 3.0f), 0.0f));
	lineObject1 = new GameObject("lineObject1", Transform(), ProjectionMode::ORTHOGRAPHIC);
	circleObject = new GameObject("circlePain", Transform(glm::vec2(2.0f, -0.45f), glm::vec2(2.0f, 2.0f)));
	circleObject2 = new GameObject("circlePain2", Transform(glm::vec2(-2.0f, -0.45f), glm::vec2(2.0f, 2.0f)));


	lineObject1->AddComponent(new LineRenderer(glm::vec2(0.5f, 0.5f), glm::vec2(-0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.324f, 1.0f), 1.0f));
	spriteObject1->AddComponent(new SpriteSheet(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("sheet.png"), 32.0f, 45.0f, 2.0f, 4.0f, glm::vec2(0, 0)));
	font->AddComponent(new FontRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "gerni gerni"));
	circleObject->AddComponent(new CircleRenderer(glm::vec4(1.0f, 0.0f, 0.1f, 1.0f), 1.0f, 0.005f));
	circleObject2->AddComponent(new CircleRenderer(DataPool::GetTexture("error_texture_256x256.png"),glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.005f));


	this->AddGameObjectToLayer(spriteObject1);
	this->AddGameObjectToLayer(lineObject1);
	this->AddGameObjectToLayer(font);
	this->AddGameObjectToLayer(circleObject);
	this->AddGameObjectToLayer(circleObject2);
}

void ForegroundLayer::OnDetach()
{

}

void ForegroundLayer::Update(const float dt)
{
	static bool con = true;

	static float x = 0;
	static int tex = 0;

	if (Input::IsKeyPressed(KEY_Y) && con && false)
	{
		con = false;
		character2->Stop();
	}
	if (Input::IsKeyPressed(KEY_T) && !con && false)
	{
		con = true;
	}

	x += dt / 8;

	tex = (int)(x * 100);

	if (tex % 8 == 0)
	{
		x = 0.01f;
	}

	spriteObject1->GetComponent<SpriteSheet>()->ChangeSprite(glm::vec2(tex, 0));
}

void ForegroundLayer::Imgui(const float dt)
{
}

void ForegroundLayer::OnEvent(Event& event)
{
	
}