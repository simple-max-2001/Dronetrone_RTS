#pragma once

#include "core/entity.h"


class Component
{
public:
	virtual void tick(double dt) = 0;

	void setParent(Entity* parent);

	Entity* getParent() const;

private:
	Entity* parent_ = nullptr;
};
