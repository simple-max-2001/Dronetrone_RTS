#include "core/world.h"

#include "core/buildings/hq.h"


World::World()
{
	spawnEntity<HQ>(EntityOwner::Player1);
	spawnEntity<HQ>(EntityOwner::Player2);
}

void World::tick(double dt)
{
	if (worldState_ != WorldState::Running)
	{
		return;
	}

	for (size_t i = 0; i < entities_.size(); i++)
	{
		entities_[i]->tick(dt);
	}
}

void World::destroyEntity(EntityId entityID)
{
	for (size_t i = 0; i < entities_.size(); i++)
	{
		if (entities_[i]->getEntityID() == entityID)
		{
			entities_.erase(entities_.begin() + i);
			events_.push(Event{ EventType::EntityDestroyed, entityID });
			return;
		}
	}
}

const std::vector<std::unique_ptr<Entity>>& World::getEntities() const
{
	return entities_;
}

WorldState World::getWorldState() const
{
	return worldState_;
}

Event World::getEvent()
{

	Event event{};

	if (events_.empty())
	{
		return event;
	}

	event = events_.front();
	events_.pop();
	return event;
}

EntityId World::getEntityID()
{
	return nextEntityID_++;
}
