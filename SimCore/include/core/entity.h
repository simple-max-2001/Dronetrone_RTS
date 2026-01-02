#pragma once

#include "simcore_types.h"

class Entity
{
public:
	Entity(EntityId entityID, 
		   EntityType entityType,
		   EntityOwner entityOwner = EntityOwner::Neutral);

	virtual void tick(double dt) = 0;

	EntityId getEntityID() const;

	EntityType getEntityType() const;

	EntityOwner getEntityOwner() const;

private:
	EntityId entityID_;
	EntityType entityType_;
	EntityOwner entityOwner_;
};