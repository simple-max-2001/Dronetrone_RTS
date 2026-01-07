#pragma once

#include "core/entity.h"

#include <memory>
#include <queue>
#include <random>
#include <vector>


class World
{
public:
	World(WorldInfo worldInfo);

	void tick(double dt);

	template<typename T>
	EntityId spawnEntity(EntityOwner owner, const Pose& pose);

	void destroyEntity(EntityId entityID);

	const std::vector<std::unique_ptr<Entity>>& getEntities() const;

	WorldState getWorldState() const;

	Event getEvent();

private:
	WorldState checkWorldState();

	EntityId getEntityID();

private:
	std::mt19937_64 rng_;

	EntityId nextEntityID_{};

	WorldInfo worldInfo_{};
	WorldState worldState_{ WorldState::Running };

	std::vector<std::unique_ptr<Entity>> entities_{};

	std::queue<Event> events_{};
};

template<typename T>
EntityId World::spawnEntity(EntityOwner owner, const Pose& pose)
{
	// Create entity with new ID
	EntityId id = getEntityID();
	std::unique_ptr<T> ptr = std::make_unique<T>(this, id, owner);

	// Set entitie's initial pose
	ptr->setPose(pose);

	// Add to list of entities
	entities_.emplace_back(std::move(ptr));

	// Add new event
	events_.push(Event{ EventType::EntityCreated, id });

	// Return entities ID
	return id;
}
