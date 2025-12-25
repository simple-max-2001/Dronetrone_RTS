#pragma once
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

    struct WorldSnapshot
    {
        uint64_t frame = 0;
        double time = 0;

        size_t entitiesCount = 0;
    };

#ifdef __cplusplus
}
#endif
