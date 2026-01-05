#include "core/world.h"
#include "core/units/ugv.h"


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

EntityId World::spawnUnit()
{
	EntityId id = getEntityID();
	auto ptr = std::make_unique<UGV>(id, EntityOwner::Player1);
	entities_.emplace_back(std::move(ptr));

	events_.push(Event{ EventType::EntityCreated, id });

	return id;
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
