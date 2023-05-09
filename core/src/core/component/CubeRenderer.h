#pragma once
#include "_Core.h"

#include "generic/Component.h"

namespace core
{
	class CubeRenderer : public RenderComponent
	{
	public:
		CubeRenderer();
		~CubeRenderer() override;

		void OnStart() override;
		void OnStop() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	};
}
