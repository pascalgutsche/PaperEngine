#include "_Game.h"
#include "UILayer.h"

UILayer::UILayer()
{
	fpsLabel = nullptr;
	coordLabel = nullptr;
	yawLabel = nullptr;
	pitchLabel = nullptr;
}

UILayer::~UILayer()
{
}

void UILayer::OnAttach()
{
	Panel* left = new Panel(glm::vec4(0.7f), Transform(glm::vec2(0.8f, 0.0f), glm::vec2(0.4f, 0.7f)), Type::Rectangle);
	AddUIObject(left, ProjectionMode::SCREEN);

	Label* fpslabelText = new Label("FPS: ", glm::vec4(0.9f), Transform(glm::vec2(-0.5f, 0.8f), glm::vec2(0.1f, 0.1f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(fpslabelText);

	fpsLabel = new Label("0", glm::vec4(0.9f), Transform(glm::vec2(0.5f, 0.8f), glm::vec2(0.1f, 0.1f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(fpsLabel);

	Label* positionLabel = new Label("Position:", glm::vec4(0.9f), Transform(glm::vec2(-0.7f, 0.5f), glm::vec2(0.08f, 0.08f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(positionLabel);

	Label* yawLabelText = new Label("Yaw:", glm::vec4(0.9f), Transform(glm::vec2(-0.7f, 0.3f), glm::vec2(0.08f, 0.08f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(yawLabelText);

	Label* pitchLabelText = new Label("Pitch:", glm::vec4(0.9f), Transform(glm::vec2(-0.7f, 0.1f), glm::vec2(0.08f, 0.08f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(pitchLabelText);

	coordLabel = new Label("N/A", glm::vec4(0.9f), Transform(glm::vec2(0.3f, 0.48f), glm::vec2(0.08f, 0.08f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(coordLabel);

	yawLabel = new Label("N/A", glm::vec4(0.9f), Transform(glm::vec2(0.3f, 0.28f), glm::vec2(0.08f, 0.08f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(yawLabel);

	pitchLabel = new Label("N/A", glm::vec4(0.9f), Transform(glm::vec2(0.3f, 0.08f), glm::vec2(0.08f, 0.08f)), DataPool::GetFont("mononoki.ttf"));
	left->AddChildObject(pitchLabel);

	Entity* error = new Entity("error", Transform(glm::vec2(-3.0f, 0.0f)));
	error->AddComponent(new SpriteRenderer(glm::vec4(1.0f), DataPool::GetTexture("error_texture_256x256.png"), 1.0f, Geometry::RECTANGLE));
	AddEntityToLayer(error);

	Entity* placeholder = new Entity("placeholder", Transform(glm::vec2(3.0f, 0.0f)));
	placeholder->AddComponent(new SpriteRenderer(glm::vec4(1.0f), DataPool::GetTexture("placeholder_texture_256x256.png"), 1.0f, Geometry::RECTANGLE));
	AddEntityToLayer(placeholder);

	
}

void UILayer::OnDetach()
{
}

void UILayer::Update(const float dt)
{
	Shr<Camera> camera = Application::GetActiveScene()->GetCamera();
	std::stringstream sscoord;
	sscoord << glm::round(camera->position.x * 10) / 10 << ", " << glm::round(camera->position.y * 10) / 10 << ", " << glm::round(camera->position.z * 10) / 10;
	coordLabel->text = sscoord.str();

	std::stringstream ssyaw;
	ssyaw << glm::round(camera->yaw * 10) / 10;
	yawLabel->text = ssyaw.str();

	std::stringstream sspitch;
	sspitch << glm::round(camera->pitch * 10) / 10;
	pitchLabel->text = sspitch.str();


	static float time = 0.1f;
	time -= Application::GetDT();
	if (time < 0)
	{
		std::stringstream ssfps;
		ssfps << glm::round(1 / Application::GetDT() * 10) / 10;
		fpsLabel->text = ssfps.str();
		time = 0.1f;
	}
}

void UILayer::OnEvent(Event& event)
{
}
