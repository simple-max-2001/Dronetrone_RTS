#include <simcore_api.h>
#include <simcore_types.h>

#include <iostream>

int main()
{
    auto* sim = sim_create();

    for (int i = 0; i < 5; ++i)
    {
        sim_tick(sim);
        const auto* ws = sim_get_world_snapshot(sim);
        std::cout << "Frame: " << ws->frame << "; Time: " << ws->time << " s; Entities count: " << ws->entitiesCount << "\n";
    }

    sim_destroy(sim);
    return 0;
}
