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

	using PlayerID = uint8_t;

    // Some constant expressions for player ID
    constexpr PlayerID NeutralPlayer = 0;
    constexpr PlayerID Player1 = 1;
    constexpr PlayerID Player2 = 2;
    constexpr PlayerID Player3 = 3;
    constexpr PlayerID Player4 = 4;

    using TeamID = uint8_t;
    
    enum class EventType : uint8_t
    {
		EntityNone,
        EntityCreated,
        EntityDestroyed,
    };

    struct Event
    {
        EventType type{};
        EntityId id{};
    };
    
    enum class WorldStateType : uint8_t
    {
        Running,
        Win,
        Draw,
    };

    struct WorldState
    {
        WorldStateType state{};
        PlayerID winner{};
	};

    SIMCORE_TYPES const char* worldStateTypeToString(WorldStateType state);

	enum class EntityType : uint8_t
    {
        Unit,
        Building,
	};

    SIMCORE_TYPES const char* entityTypeToString(EntityType type);

    struct Pose {
		int32_t x   = 0;    // position at X in centimeters
		int32_t y   = 0;    // position at Y in centimeters
        int32_t yaw = 0;    // yaw angle in degrees * 100 (0..360_00)
    };

	enum class MatchMode : uint8_t
    {
		Deathmatch,
        CapturePoints,
		Scenario,
	};

    struct MapInfo
    {
		int32_t width;  // width in centimeters
		int32_t height; // height in centimeters

        const Pose* spawnPoints;
		size_t spawnPointsCount;
    };

    struct WorldInfo
    {
		MatchMode matchMode{};
		uint8_t maxPlayers = 2;
		MapInfo mapInfo{};
        uint64_t seed = 42;
    };

    struct EntitySnapshot
    {
        EntityId id{};
		EntityType type{};
        PlayerID owner{};
		Pose pose{};
    };

    struct WorldSnapshot
    {
        uint64_t frame = 0;
        double time = 0;

		WorldStateType worldState = WorldStateType::Running;
        TeamID teamWinner{};

        size_t entitiesCount = 0;
        EntitySnapshot* entities = nullptr;
    };

#ifdef __cplusplus
}
#endif
