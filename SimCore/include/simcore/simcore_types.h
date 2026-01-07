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
        Player4,
    };

    SIMCORE_TYPES const char* entityOwnerToString(EntityOwner owner);
    
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
    
    enum class WorldState : uint8_t
    {
        Running,
        Team1Win,
        Team2Win,
        Team3Win,
        Draw,
    };

    SIMCORE_TYPES const char* worldStateToString(WorldState state);

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
		int32_t width  = 240'00; // width in centimeters
		int32_t height = 240'00; // height in centimeters

        const Pose* spawnPoints = new Pose[]{ { 20'00,  20'00,   0'00},
                                              {190'00, 190'00, 180'00} };
		size_t spawnPointsCount = 2;
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
		EntityOwner owner{};
		Pose pose{};
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
