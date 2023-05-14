#pragma once
#include "Engine.h"

#include "generic/Component.h"

namespace engine
{
	class LightComponent : public Component
	{
	public:
		LightComponent();
		~LightComponent() override = default;

		void OnStart() override;
		void OnStop() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	};
}
