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
        std::cout << "Frame: " << ws->frame << "; Time: " << ws->time << " s; State: " << worldStateToString(ws->worldState) << "; Entities count: " << ws->entitiesCount << "\n";

        for (size_t i = 0; i < ws->entitiesCount; i++)
        {
            std::cout << "\tEntity #" << i << ": ID=" << ws->entities[i].id << "\n";
        }

        while (true)
        {
            const auto* event = sim_get_event(sim);
            if (event->type == EventType::EntityNone)
            {
                break;
            }
			std::cout << "\tEvent: Type=" << static_cast<int>(event->type) << "; Entity ID=" << event->id << "\n";
        }
    }

    sim_destroy(sim);
    return 0;
}
