#pragma once

#include "core/entity.h"

#include <vector>

class World
{
public:
	void tick(double dt);

	EntityId spawnEntity();
	void destroyEntity(EntityId entityID);

	const std::vector<Entity>& getEntities() const;

	WorldState getWorldState() const;

private:
	EntityId getEntityID();

private:
	EntityId nextEntityID_{};

	WorldState worldState_{ WorldState::Running };

	std::vector<Entity> entities_{};
};