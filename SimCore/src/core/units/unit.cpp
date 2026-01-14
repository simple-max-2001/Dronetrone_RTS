#include "core/units/unit.h"


Unit::Unit(World* world, EntityId entityID, PlayerID entityOwner) : 
	Entity(world, entityID, EntityType::Unit, entityOwner)
{
	healthComponent_ = std::make_unique<HealthComponent>();
}

HealthComponent* Unit::getHealthComponent()
{
	return nullptr;
}
