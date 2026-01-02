#pragma once

#include "core/entity.h"

#include <memory>
#include <vector>

class World
{
public:
	void tick(double dt);

	EntityId spawnUnit();

	void destroyEntity(EntityId entityID);

	const std::vector<std::unique_ptr<Entity>>& getEntities() const;

	WorldState getWorldState() const;

private:
	EntityId getEntityID();

private:
	EntityId nextEntityID_{};

	WorldState worldState_{ WorldState::Running };

	std::vector<std::unique_ptr<Entity>> entities_{};
};