#include "simcore_types.h"


SIMCORE_TYPES const char* entityOwnerToString(EntityOwner owner)
{
    switch (owner)
    {
    case EntityOwner::Neutral:
        return "Neutral";
    case EntityOwner::Player1:
        return "Player 1";
    case EntityOwner::Player2:
        return "Player 2";
    case EntityOwner::Player3:
        return "Player 3";
    default:
        break;
    }
}

SIMCORE_TYPES const char* worldStateToString(WorldState state)
{
    switch (state)
    {
    case WorldState::Running:
        return "Running";
    case WorldState::Team1Win:
        return "Team's 1 Win";
    case WorldState::Team2Win:
        return "Team's 2 Win";
    case WorldState::Draw:
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
