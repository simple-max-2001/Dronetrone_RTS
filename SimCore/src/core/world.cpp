#include "core/world.h"


void World::tick(double dt)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i].tick(dt);
	}
}

EntityId World::spawnEntity()
{
	EntityId id = getEntityID();
	entities.emplace_back(id);
	return id;
}

void World::destroyEntity(EntityId entityID)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i].getEntityID() == entityID)
		{
			entities.erase(entities.begin() + i);
			return;
		}
	}
}

const std::vector<Entity>& World::getEntities() const
{
	return entities;
}

WorldState World::getWorldState() const
{
	return worldState_;
}

EntityId World::getEntityID()
{
	return nextEntityID++;
}
