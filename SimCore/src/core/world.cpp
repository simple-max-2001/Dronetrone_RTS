#include "core/world.h"

#include "core/buildings/hq.h"


World::World(WorldInfo worldInfo) :
	worldInfo_(worldInfo)
{
	rng_ = std::mt19937_64(worldInfo_.seed);

	// Pick random pose from available
	// Build a pool of spawn indices and shuffle it to select unique spawn points
	// Complexity: O(N) to initialize and shuffle the pool (N = spawnPointsCount).
	// This avoids repeated sampling and the memory leak from the previous raw array allocation.
	{
		size_t players = worldInfo.maxPlayers;
		size_t poolCount = worldInfo_.mapInfo.spawnPointsCount;
		if (players > poolCount)
		{
			// Clamp to available spawn points to avoid undefined behavior / infinite loops
			players = poolCount;
		}

		std::vector<size_t> pool(poolCount);
		for (size_t i = 0; i < poolCount; i++)
		{
			pool[i] = i;
		}

		// Shuffle pool using the world's RNG and take the first `players` indices
		std::shuffle(pool.begin(), pool.end(), rng_);

		std::vector<size_t> spawnPoseIdxs;
		spawnPoseIdxs.reserve(players);
		for (size_t i = 0; i < players; i++)
		{
			spawnPoseIdxs.push_back(pool[i]);
		}

		// Spawn HQs of players
		for (size_t i = 0; i < players; i++)
		{
			Pose pose = worldInfo_.mapInfo.spawnPoints[spawnPoseIdxs[i]];
			spawnEntity<HQ>(EntityOwner::Player1, pose);
		}
	}
}

void World::tick(double dt)
{
	if (worldState_ != WorldState::Running)
	{
		return;
	}

	for (size_t i = 0; i < entities_.size(); i++)
	{
		entities_[i]->tick(dt);
	}

	worldState_ = checkWorldState();
}

void World::destroyEntity(EntityId entityID)
{
	for (size_t i = 0; i < entities_.size(); i++)
	{
		if (entities_[i]->getEntityID() == entityID)
		{
			entities_.erase(entities_.begin() + i);
			events_.push(Event{ EventType::EntityDestroyed, entityID });
			return;
		}
	}
}

const std::vector<std::unique_ptr<Entity>>& World::getEntities() const
{
	return entities_;
}

WorldState World::getWorldState() const
{
	return worldState_;
}

Event World::getEvent()
{

	Event event{};

	if (events_.empty())
	{
		return event;
	}

	event = events_.front();
	events_.pop();
	return event;
}

WorldState World::checkWorldState()
{
	// Check HQs and determine winner
	// TODO: implement team distribution
	for (size_t i = 0; i < entities_.size(); i++)
	{
		Entity* entity = entities_[i].get();
		if (entity->getEntityType() == EntityType::Building)
		{
			// TODO: check for HQ type
			Building* building = static_cast<Building*>(entity);
			HealthComponent* health = building->getHealthComponent();
			if (!health->isAlive())
			{
				// HQ destroyed
				if (building->getEntityOwner() == EntityOwner::Player1)
				{
					return WorldState::Team2Win;
				}
				else if (building->getEntityOwner() == EntityOwner::Player2)
				{
					return WorldState::Team1Win;
				}
			}
		}
	}

	return WorldState::Running;
}

EntityId World::getEntityID()
{
	return nextEntityID_++;
}
