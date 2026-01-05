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
        Draw,
    };

    SIMCORE_TYPES const char* worldStateToString(WorldState state);

	enum class EntityType : uint8_t
    {
        Unit,
        Building
	};

    SIMCORE_TYPES const char* entityTypeToString(EntityType type);

    struct Pose {
		int32_t x;      // position at X in centimeters
		int32_t y;      // position at Y in centimeters
        int32_t yaw;    // yaw angle in degrees * 100 (0..360_00)
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
