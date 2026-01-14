#include "simcore/simcore_api.h"

#include "core/units/ugv.h"
#include "core/world.h"

#include <cstdint>


class SimCoreImpl
{
public:
    SimCoreImpl(WorldInfo* wi)
    {
        worldInfo_ = wi ? *wi : WorldInfo
        {
            .matchMode  = MatchMode::Deathmatch,
            .maxPlayers = 2,
            .mapInfo    = MapInfo
            {
                .width  = 250'00,
                .height = 250'00,
                .spawnPoints = new Pose[]
                {
                    Pose{
                        
                    }
                }
            },
            .seed       = 42,
        };
    }

    ~SimCoreImpl()
    {
        stop();
    }

    void reset()
    {
        stop();

        world_ = std::make_unique<World>(WorldInfo{});
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
        // If world wasn't created, do nothing
        if (!world_) return;

        // Update world
        world_->tick(fixedDelta_);

        // Update timers
        frame_++;
        timestamp_ += fixedDelta_;

        // TEST: Spawn unit at frame 2
        if (frame_ == 2)
        {
            world_->spawnEntity<UGV>(Player1, Pose{});
            world_->spawnEntity<UGV>(Player2, Pose{});
		}

		// TEST: Destroy unit at frame 4
        if (frame_ == 4)
        {
            world_->destroyEntity(world_->getEntities().at(1)->getEntityID());
        }

        // -------- Update world's snapshot --------
        // Get current timestamp and frame id
        worldSnapshot_.time = timestamp_;
        worldSnapshot_.frame = frame_;

		worldSnapshot_.worldState = world_->getWorldState();

        // Make entities snapshots
        const auto& entities = world_->getEntities();
        worldSnapshot_.entitiesCount = entities.size();

        worldSnapshot_.entities = new EntitySnapshot[worldSnapshot_.entitiesCount];
        size_t i = 0;
        for (const auto& pair : entities)
        {
            // Write inforamtion to entity snapshot
			const Entity* entity = pair.second.get();
            worldSnapshot_.entities[i++] = EntitySnapshot{
                .id    = entity->getEntityID(),
                .type  = entity->getEntityType(),
				.owner = entity->getOwner(),
                .pose  = entity->getPose()
            };
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
    uint64_t frame_ = 0;
    double timestamp_ = 0;

    double fixedDelta_ = 0.02;

    std::unique_ptr<World> world_;
    WorldInfo worldInfo_;

    WorldSnapshot worldSnapshot_{};
	Event event_{};
};

SIMCORE_API SimHandle sim_create(WorldInfo* wi)
{
    auto* sim = new SimCoreImpl(wi);
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