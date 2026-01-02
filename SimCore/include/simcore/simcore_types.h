#pragma once

#ifdef _WIN32
#ifdef SIMCORE_EXPORT
#define SIMCORE_TYPES __declspec(dllexport)
#else
#define SIMCORE_TYPES __declspec(dllimport)
#endif
#else
#define SIMCORE_TYPES
#endif

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
    using EntityId = uint64_t;

    enum class EntityOwner : uint8_t
    {
        Neutral,
        Player1,
        Player2,
        Player3,
    };
    
    enum class WorldState : uint8_t
    {
        Running,
        Team1Win,
        Team2Win,
        Draw,
    };

    SIMCORE_TYPES const char* worldStateToString(WorldState state);

	enum class EntityType : uint8_t
    {
        Unit,
        Building
	};

    struct EntitySnapshot
    {
        EntityId id{};
    };

    struct WorldSnapshot
    {
        uint64_t frame = 0;
        double time = 0;

		WorldState worldState = WorldState::Running;

        size_t entitiesCount = 0;
        EntitySnapshot* entities = nullptr;
    };

#ifdef __cplusplus
}
#endif
