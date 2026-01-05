#include "simcore/simcore_api.h"
#include "core/world.h"

#include <cstdint>
#include <random>

class SimCoreImpl
{
public:
    SimCoreImpl(uint64_t seed = 42) : rng_(seed)
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
		world_->spawnUnit();
    }

    void stop()
    {
        frame_ = 0;
        timestamp_ = 0;
        world_.reset();

        worldSnapshot_ = WorldSnapshot();
		event_ = Event{};
    }

    void tick()
    {
        // Якщо світ не було створено, то нічого не робимо
        if (!world_) return;

        // Рух юнітів та оновлення світу
        world_->tick(fixedDelta_);

        // Оновлення таймеру
        frame_++;
        timestamp_ += fixedDelta_;

        // TEST: Spawn unit at frame 2
        if (frame_ == 2)
        {
            world_->spawnUnit();
		}

		// TEST: Destroy unit at frame 4
        if (frame_ == 4)
        {
            world_->destroyEntity(world_->getEntities()[0]->getEntityID());
        }

        // -------- Оновлюємо снапшот світу --------
        // Отримуємо поточні фрейм та мітку часу
        worldSnapshot_.time = timestamp_;
        worldSnapshot_.frame = frame_;

		worldSnapshot_.worldState = world_->getWorldState();

        // Отримуємо інформацію про кількість юнітів
        const auto& entities = world_->getEntities();
        worldSnapshot_.entitiesCount = entities.size();

        worldSnapshot_.entities = new EntitySnapshot[worldSnapshot_.entitiesCount];
        for (size_t i = 0; i < worldSnapshot_.entitiesCount; i++)
        {
            // Заповнюємо інформацію про кожен юніт у снапшоті
			const Entity* entity = entities[i].get();
            worldSnapshot_.entities[i] = EntitySnapshot{entity->getEntityID(),
                                                        entity->getEntityType(),
				                                        entity->getEntityOwner(),
                                                        entity->getPose()};
        }
    }
    
    const WorldSnapshot* sim_get_world_snapshot()
    {
        return &worldSnapshot_;
    }

    const Event* sim_get_event()
    {
        if (world_)
			event_ = world_->getEvent();

        return &event_;
	}

private:
    std::mt19937_64 rng_;

    uint64_t frame_ = 0;
    double timestamp_ = 0;

    double fixedDelta_ = 0.02;

    std::unique_ptr<World> world_;

    WorldSnapshot worldSnapshot_{};
	Event event_{};
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

SIMCORE_API const Event* sim_get_event(SimHandle h)
{
    auto* sim = static_cast<SimCoreImpl*>(h);
    return sim->sim_get_event();
}