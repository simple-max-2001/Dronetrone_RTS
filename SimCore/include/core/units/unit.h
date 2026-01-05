#pragma once

#include "core/entity.h"
#include "core/components/health.h"

#include <memory>


class Unit : public Entity
{
public:
	Unit(EntityId entityID, EntityOwner entityOwner);

	HealthComponent* getHealthComponent();

private:
	std::unique_ptr<HealthComponent> healthComponent_;
};
