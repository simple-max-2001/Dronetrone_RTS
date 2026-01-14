#include "core/buildings/building.h"


Building::Building(World* world, EntityId entityID, PlayerID entityOwner) :
	Entity(world, entityID, EntityType::Building, entityOwner)
{
	healthComponent_ = std::make_unique<HealthComponent>();
}

HealthComponent* Building::getHealthComponent()
{
	return healthComponent_.get();
}
