#include "core/world.h"


void World::tick(double dt)
{
	if (worldState_ != WorldState::Running)
	{
		return;
	}

	for (size_t i = 0; i < entities_.size(); i++)
	{
		entities_[i].tick(dt);
	}
}

EntityId World::spawnEntity()
{
	EntityId id = getEntityID();
	entities_.emplace_back(id);
	return id;
}

void World::destroyEntity(EntityId entityID)
{
	for (size_t i = 0; i < entities_.size(); i++)
	{
		if (entities_[i].getEntityID() == entityID)
		{
			entities_.erase(entities_.begin() + i);
			return;
		}
	}
}

const std::vector<Entity>& World::getEntities() const
{
	return entities_;
}

WorldState World::getWorldState() const
{
	return worldState_;
}

EntityId World::getEntityID()
{
	return nextEntityID_++;
}
