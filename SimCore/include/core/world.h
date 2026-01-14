#pragma once

#include "core/entity.h"

#include <memory>
#include <queue>
#include <random>
#include <map>


class World
{
public:
	World(WorldInfo worldInfo);

	void tick(double dt);

	template<typename T>
	EntityId spawnEntity(PlayerID owner, const Pose& pose);

	void destroyEntity(EntityId entityID);

	const std::map<EntityId, std::unique_ptr<Entity>>& getEntities() const;

	WorldStateType getWorldState() const;

	Event getEvent();

private:
	WorldStateType checkWorldState();

	EntityId getEntityID();

private:
	std::mt19937_64 rng_;

	EntityId nextEntityID_{};

	WorldInfo worldInfo_{};
	WorldStateType worldState_{ WorldStateType::Running };

	std::map<EntityId, std::unique_ptr<Entity>> entities_{};

	std::queue<Event> events_{};
};

template<typename T>
EntityId World::spawnEntity(PlayerID owner, const Pose& pose)
{
	// Create entity with new ID
	EntityId id = getEntityID();
	std::unique_ptr<T> ptr = std::make_unique<T>(this, id, owner);

	// Set entitie's initial pose
	ptr->setPose(pose);

	// Add to list of entities
	entities_.emplace(id, std::move(ptr));

	// Add new event
	events_.push(Event{ EventType::EntityCreated, id });

	// Return entities ID
	return id;
}
