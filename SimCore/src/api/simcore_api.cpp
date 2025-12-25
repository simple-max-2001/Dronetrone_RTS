#include <simcore/simcore_api.h>
#include <cstdint>

class SimCoreImpl
{
public:
    uint64_t tick = 0;
};

SIMCORE_API SimHandle sim_create()
{
    return new SimCoreImpl{};
}

SIMCORE_API void sim_destroy(SimHandle h)
{
    delete static_cast<SimCoreImpl*>(h);
}

SIMCORE_API void sim_tick(SimHandle h)
{
    auto* sim = static_cast<SimCoreImpl*>(h);
    sim->tick++;
}
