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
		globalPos = parent->globalPos + transform.position * transform.scale;
	}
}
