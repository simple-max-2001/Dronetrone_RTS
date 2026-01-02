#include "core/entity.h"


Entity::Entity(EntityId entityID, EntityType entityType, EntityOwner entityOwner) : 
	entityID_(entityID), entityType_(entityType), entityOwner_(entityOwner)
{

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
