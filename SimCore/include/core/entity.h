#pragma once

#include "simcore_types.h"


class World;

class Entity
{
public:
	Entity(World* world,
		   EntityId entityID, 
		   EntityType entityType,
		   EntityOwner entityOwner = EntityOwner::Neutral);

	~Entity() = default;

	World* getWorld() const;

	virtual void tick(double dt) = 0;

	EntityId getEntityID() const;

	EntityType getEntityType() const;

	EntityOwner getEntityOwner() const;

	void setPose(const Pose& pose);

	const Pose& getPose() const;

private:
	World* world_{};

	EntityId entityID_;
	EntityType entityType_;
	EntityOwner entityOwner_;

	Pose pose_{};
};