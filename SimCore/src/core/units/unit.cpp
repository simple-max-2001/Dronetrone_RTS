#include "core/units/unit.h"

Unit::Unit(EntityId entityID, EntityOwner entityOwner) : Entity(entityID, EntityType::Unit, entityOwner)
{
	healthComponent_ = std::make_unique<HealthComponent>();
}

HealthComponent* Unit::getHealthComponent()
{
	return nullptr;
}
