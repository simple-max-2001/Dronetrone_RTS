#pragma once

#include "unit.h"

class UGV : public Unit
{
public:
	UGV(World* world, EntityId entityID, PlayerID entityOwner);

	virtual void tick(double dt) override;
};