#pragma once

#include "core/entity.h"

#include <memory>
#include <vector>
#include <queue>


class World
{
public:
	World();

	void tick(double dt);

	template<typename T>
	EntityId spawnEntity(EntityOwner owner);

	void destroyEntity(EntityId entityID);

	const std::vector<std::unique_ptr<Entity>>& getEntities() const;

	WorldState getWorldState() const;

	Event getEvent();

private:
	EntityId getEntityID();

private:
	EntityId nextEntityID_{};

	WorldState worldState_{ WorldState::Running };

	std::vector<std::unique_ptr<Entity>> entities_{};

	std::queue<Event> events_{};
};

template<typename T>
EntityId World::spawnEntity(EntityOwner owner)
{
	EntityId id = getEntityID();
	auto ptr = std::make_unique<T>(this, id, owner);
	entities_.emplace_back(std::move(ptr));

	events_.push(Event{ EventType::EntityCreated, id });

	return id;
}
