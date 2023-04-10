#pragma once
#include "_Core.h"

#include "generic/GameObject.h"

namespace core
{
	enum class Type
	{
		Rectangle,
		Triangle,
		Circle
	};

	class UIObject
	{
		friend class Layer;
	public:
		

		UIObject(const Transform& transform);
		virtual ~UIObject();

		virtual void Render() = 0;

		void AddChildObject(UIObject* uiObject);

		UIObject* GetParentObject() const;

		core_id GetObjectID() const;
		std::vector<UIObject*>& GetChildObjects();
		

		Transform transform;
	protected:
		glm::vec2 globalPos;
		ProjectionMode mode;

		core_id coreID = 0;
		UIObject* parent = nullptr;
		std::vector<UIObject*> childOjects;

		void CalculateGlobalCoords();
	};
}


