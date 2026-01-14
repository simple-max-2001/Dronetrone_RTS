#pragma once

#include "core/buildings/building.h"


class HQ : public Building
{
public:
	HQ(World* world, EntityId entityID, PlayerID entityOwner);

	virtual void tick(double dt) override;
};
