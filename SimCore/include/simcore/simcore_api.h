#pragma once

#ifdef _WIN32
#ifdef SIMCORE_EXPORT
#define SIMCORE_API __declspec(dllexport)
#else
#define SIMCORE_API __declspec(dllimport)
#endif
#else
#define SIMCORE_API
#endif

#include "simcore_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef void* SimHandle;

    SIMCORE_API SimHandle sim_create();
    SIMCORE_API void sim_destroy(SimHandle);
    SIMCORE_API void sim_tick(SimHandle);
    SIMCORE_API const WorldSnapshot* sim_get_world_snapshot(SimHandle);
	SIMCORE_API const Event* sim_get_event(SimHandle);

#ifdef __cplusplus
}
#endif
