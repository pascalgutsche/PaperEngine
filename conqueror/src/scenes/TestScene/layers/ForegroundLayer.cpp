#include "_Game.h"
#include "ForegroundLayer.h"

#include "component/CircleRenderer.h"
#include "component/SpriteSheet.h"


void ForegroundLayer::OnAttach()
{
	character1 = new GameObject("character1", Transform(glm::vec2(3.0f, 3.0f), glm::vec2(2.0f, 2.0f)));
	character2 = new GameObject("character2", Transform(glm::vec2(0.0f, 3.0f), glm::vec2(2.0f, -1.0f), 115.0f));
	object1 = new GameObject("object1", Transform(glm::vec2(-3, 3), glm::vec2(2.0f, 2.0f)));

	circleObject = new GameObject("circle", Transform(glm::vec2(5.6345f, 0.6f), glm::vec2(3.0f, 6.0f)));

	spriteObject1 = new GameObject("spritePain", Transform(glm::vec2(0.436f, 3.32432f), glm::vec2(1.5, 3.0f), 0.0f));

	// nobody cares about the transform for lines
	// TODO: annoy pascal that his code sucks
	// ps: mine isn't any better
	lineObject1 = new GameObject("lineObject1", Transform(), ProjectionMode::ORTHOGRAPHIC);

	circleObject->AddComponent(new CircleRenderer(circleObject->transform.position, circleObject->transform.scale, glm::vec4(0.342340f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f, 0.05f));
	character1->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("james_webb.png"), Geometry::RECTANGLE));
	character2->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("placeholder_texture_256x256.png"), Geometry::TRIANGLE));
	object1->AddComponent(new SpriteRenderer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("antimarx.png"), Geometry::RECTANGLE));
	lineObject1->AddComponent(new LineRenderer(glm::vec2(0.5f, 0.5f), glm::vec2(-0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.324f, 1.0f), 1.0f));
	spriteObject1->AddComponent(new SpriteSheet(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), DataPool::GetTexture("sheet.png"), 32.0f, 45.0f, 2.0f, 4.0f, glm::vec2(0, 0)));
	
	this->AddGameObjectToLayer(spriteObject1);
	this->AddGameObjectToLayer(character1);
	this->AddGameObjectToLayer(character2);
	this->AddGameObjectToLayer(object1);
	this->AddGameObjectToLayer(lineObject1);
	this->AddGameObjectToLayer(circleObject);
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