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

private:
	EntityId getEntityID();

private:
	EntityId nextEntityID{};

	std::vector<Entity> entities{};
};