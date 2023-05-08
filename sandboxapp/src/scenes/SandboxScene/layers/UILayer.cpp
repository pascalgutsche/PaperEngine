#include "_Game.h"
#include "UILayer.h"

UILayer::UILayer()
{

}

UILayer::~UILayer()
{
}

void UILayer::OnAttach()
{
	Panel* left = new Panel(glm::vec4(0.7f), Transform(glm::vec2(0.8f, 0.0f), glm::vec2(0.4f, 0.7f)), Type::Rectangle);
	AddUIObject(left, ProjectionMode::SCREEN);

	Label* label = new Label("SandboxApp", glm::vec4(0.8f, 0.2f, 0.3f, 0.9f), Transform(glm::vec2(0.0f, 0.8f), glm::vec2(0.15f, 0.15f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(label);

	GameObject* error = new GameObject("error", Transform(glm::vec2(-1.0f, 0.0f)));
	error->AddComponent(new SpriteRenderer(glm::vec4(1.0f), DataPool::GetTexture("error_texture_256x256.png"), 1.0f, Geometry::RECTANGLE));
	AddGameObjectToLayer(error);

	GameObject* placeholder = new GameObject("placeholder", Transform(glm::vec2(1.0f, 0.0f)));
	placeholder->AddComponent(new SpriteRenderer(glm::vec4(1.0f), DataPool::GetTexture("placeholder_texture_256x256.png"), 1.0f, Geometry::RECTANGLE));
	AddGameObjectToLayer(placeholder);
}

void UILayer::OnDetach()
{
}

void UILayer::Update(const float dt)
{
}

void UILayer::OnEvent(Event& event)
{
}
