#pragma once
#include "_Game.h"

class UILayer : public Layer
{
public:
	UILayer();
	~UILayer() override;

	void OnAttach() override;
	void OnDetach() override;
	void Update(const float dt) override;
	void OnEvent(Event& event) override;

private:
	Label* fpsLabel;
	Label* coordLabel;
	Label* yawLabel;
	Label* pitchLabel;
	Label* fovLabel;
};

