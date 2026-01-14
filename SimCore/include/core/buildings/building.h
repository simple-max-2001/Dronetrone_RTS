#pragma once

#include "core/entity.h"
#include "core/components/health.h"

#include <memory>


class Building : public Entity
{
public:
	Building(World* world, EntityId entityID, PlayerID entityOwner);

	HealthComponent* getHealthComponent();

private:
	std::unique_ptr<HealthComponent> healthComponent_;
};
