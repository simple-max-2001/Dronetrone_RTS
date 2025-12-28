#pragma once
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
    using EntityId = uint64_t;

    struct EntitySnapshot
    {
        EntityId id{};
    };

    struct WorldSnapshot
    {
        uint64_t frame = 0;
        double time = 0;

        size_t entitiesCount = 0;
        EntitySnapshot* entities = nullptr;
    };

#ifdef __cplusplus
}
#endif
