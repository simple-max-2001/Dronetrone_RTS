#include "simcore/simcore_api.h"
#include "core/world.h"

#include <cstdint>
#include <random>

class SimCoreImpl
{
public:
    SimCoreImpl(uint64_t seed = 42) : rng(seed)
    {
    }

    ~SimCoreImpl()
    {
        stop();
    }

    void reset()
    {
        stop();

        world_ = std::make_unique<World>();
    }

    void stop()
    {
        frame_ = 0;
        timestamp_ = 0;
        world_.reset();

        worldSnapshot = WorldSnapshot();
    }

    void tick()
    {
        if (!world_) return;

        // Рух юнітів та оновлення світу
        world_->tick(fixedDelta_);

        // Оновлення таймеру
        frame_++;
        timestamp_ += fixedDelta_;

        // -------- Оновлюємо снапшот світу --------
        // Отримуємо поточні фрейм та мітку часу
        worldSnapshot.time = timestamp_;
        worldSnapshot.frame = frame_;

        // Отримуємо інформацію про кількість юнітів
        const auto& entities = world_->getEntities();
        worldSnapshot.entitiesCount = entities.size();

        worldSnapshot.entities = new EntitySnapshot[worldSnapshot.entitiesCount];
        for (size_t i = 0; i < worldSnapshot.entitiesCount; i++)
        {
            // Заповнюємо інформацію про кожен юніт у снапшоті
            worldSnapshot.entities[i] = EntitySnapshot(entities.at(i).getEntityID());
        }
    }

    const WorldSnapshot* sim_get_world_snapshot()
    {
        return &worldSnapshot;
    }

private:
    std::mt19937_64 rng;

    uint64_t frame_ = 0;
    double timestamp_ = 0;

    double fixedDelta_ = 0.02;

    std::unique_ptr<World> world_;
    WorldSnapshot worldSnapshot{};
};

SIMCORE_API SimHandle sim_create(uint64_t seed)
{
    auto* sim = new SimCoreImpl(seed);
    sim->reset();
    return sim;
}

SIMCORE_API void sim_destroy(SimHandle h)
{
    auto* sim = static_cast<SimCoreImpl*>(h);
    delete sim;
}

SIMCORE_API void sim_tick(SimHandle h)
{
    auto* sim = static_cast<SimCoreImpl*>(h);
    sim->tick();
}

SIMCORE_API const WorldSnapshot* sim_get_world_snapshot(SimHandle h)
{
    auto* sim = static_cast<SimCoreImpl*>(h);
    return sim->sim_get_world_snapshot();
}