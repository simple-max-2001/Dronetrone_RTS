#pragma once

#include "unit.h"

class UGV : public Unit
{
public:
	UGV(EntityId entityID, EntityOwner entityOwner);

	virtual void tick(double dt) override;
};