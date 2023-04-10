#pragma once
#include "_Core.h"

#include "generic/Object.h"
#include "utils/DataPool.h"

namespace core
{
	enum class Type
	{
		Rectangle,
		Triangle,
		Circle
	};

	class UIObject : public Object
	{
		friend class Layer;
	public:
		

		UIObject(std::string name, const Transform& transform);
		virtual ~UIObject() override;

		virtual void Render() = 0;

		void AddChildObject(UIObject* uiObject);

		UIObject* GetParentObject() const;
		std::vector<UIObject*>& GetChildObjects();
		

	protected:
		glm::vec2 globalPos;
		ProjectionMode mode;

		UIObject* parent = nullptr;
		std::vector<UIObject*> childOjects;

		void CalculateGlobalCoords();
	};
}


