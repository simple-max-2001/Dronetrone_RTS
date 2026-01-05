#include "core/entity.h"


Entity::Entity(World* world, EntityId entityID, EntityType entityType, EntityOwner entityOwner) :
	world_(world), entityID_(entityID), entityType_(entityType), entityOwner_(entityOwner)
{

}

World* Entity::getWorld() const
{
	return world_;
}

EntityId Entity::getEntityID() const
{
	return entityID_;
}

EntityType Entity::getEntityType() const
{
	return entityType_;
}

EntityOwner Entity::getEntityOwner() const
{
	return entityOwner_;
}

void Entity::setPose(const Pose& pose)
{
	pose_ = pose;
}

const Pose& Entity::getPose() const
{
	return pose_;
}
