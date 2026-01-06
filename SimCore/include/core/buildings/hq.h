#pragma once

#include "core/buildings/building.h"


class HQ : public Building
{
public:
	HQ(World* world, EntityId entityID, EntityOwner entityOwner);

	virtual void tick(double dt) override;
};
