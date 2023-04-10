#include "_Core.h"
#include "UIObject.h"

#include "utils/Core.h"

namespace core
{
	UIObject::UIObject(const Transform& transform)
		: transform(transform)
	{
		coreID = Core::RequestID();
	}

	UIObject::~UIObject()
	{
		DataPool::AddToDelete(coreID);
	}

	void UIObject::AddChildObject(UIObject* uiObject)
	{
		uiObject->parent = this;
		childOjects.push_back(uiObject);
	}

	UIObject* UIObject::GetParentObject() const
	{
		return parent;
	}

	core_id UIObject::GetObjectID() const
	{
		return coreID;
	}

	std::vector<UIObject*>& UIObject::GetChildObjects()
	{
		return childOjects;
	}

	void UIObject::CalculateGlobalCoords()
	{
		if (!parent) return;
		globalPos = parent->globalPos + transform.position * parent->transform.scale * glm::vec2(0.5f);
	}
}
