#include "simcore_types.h"


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
