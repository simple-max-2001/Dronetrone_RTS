#include "core/entity.h"


Entity::Entity(EntityId entityID) : entityID_(entityID)
{

}

void Entity::tick(double dt)
{

}

EntityId Entity::getEntityID() const
{
	return entityID_;
}
