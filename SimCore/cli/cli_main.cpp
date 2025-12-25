#include <simcore/simcore_api.h>
#include <iostream>

int main()
{
    SimHandle sim = sim_create();

    for (int i = 0; i < 5; ++i)
    {
        sim_tick(sim);
        std::cout << "tick " << i << std::endl;
    }

    sim_destroy(sim);
    return 0;
}
