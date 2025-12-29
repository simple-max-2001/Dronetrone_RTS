#pragma once

#include "simcore_types.h"

class Entity
{
public:
	Entity(EntityId entityID);

	void tick(double dt);

	EntityId getEntityID() const;

private:
	EntityId entityID_;
};