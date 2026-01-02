#include "core/components/component.h"

void Component::setParent(Entity* parent)
{
	parent_ = parent;
}

Entity* Component::getParent() const
{
	return parent_;
}
