#include "simcore_types.h"


SIMCORE_TYPES const char* worldStateTypeToString(WorldStateType state)
{
    switch (state)
    {
    case WorldStateType::Running:
        return "Running";
    case WorldStateType::Win:
        return "Win";
    case WorldStateType::Draw:
        return "Draw";
    default:
        return "Unknown";
    }
}

SIMCORE_TYPES const char* entityTypeToString(EntityType type)
{
    switch (type)
	{
	case EntityType::Unit:
		return "Unit";
	case EntityType::Building:
		return "Building";
	default:
		return "Unknown";
	}
}
