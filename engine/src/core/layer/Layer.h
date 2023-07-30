#pragma once
#include "Engine.h"
#include "utility.h"

#include "event/Event.h"
#include "generic/Entity.h"
#include "generic/Scene.h"

#include "utils/DataPool.h"

namespace Paper
{
	class UIObject;
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: name(name) { }

		virtual ~Layer() = default;

		void Attach();
		void Detach();

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void Update(const float dt) = 0;
		virtual void Imgui(const float dt) { };
		virtual void OnEvent(Event& event) = 0;

		bool IsAttached() const { return attached; }

		void SetOverlayStatus(bool overlay) { this->overlay = overlay; }
		bool GetOverlayStatus() { return this->overlay; }

		bool GetOverlayStatus() const { return overlay; }
		std::string GetName() { return name; }
		
	protected:
		std::string name;
		bool overlay = false;
		bool attached = false;
	};
}