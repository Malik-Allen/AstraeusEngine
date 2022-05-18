#ifndef WORLD_H
#define WORLD_H

#include "Objects/ActorCore.h"

#include <vector>

namespace ECS
{
	class World;
};

class Actor;

/*
*	Wrapper Class for World Instance 
*/
class World
{
	friend class IApp;
public:
	World();
	~World();

	/*
	*	Spawns actor in world, using the spawn info
	*	@param	ActorSpawnInfo:		The information used to spawn actor
	*	@return	Actor:		Returns a reference to the actor that is spawned in the world, returns nullptr if the actor could not be spawned
	*/
	Actor* SpawnActor( ActorSpawnInfo actorSpawnInfo = ActorSpawnInfo() );

	inline ECS::World* GetECS() const
	{
		return m_world;
	};

private:
	bool OnCreate();
	void OnDestroy();
	void Update( const float deltaTime );

	ECS::World* m_world;
	Actor* m_worldParentActor;
};

#endif // !WORLD_H

