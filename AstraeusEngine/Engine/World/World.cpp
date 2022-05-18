#include "World.h"
#include "Objects/Actor.h"
#include <ECS.h>
#include <DebugLog.h>

World::World() :
	m_world( new ECS::World() ),
	m_worldParentActor( nullptr )
{}

World::~World()
{
	if( m_worldParentActor != nullptr )
	{
		delete m_worldParentActor;
		m_worldParentActor = nullptr;
	}
	if( m_world != nullptr )
	{
		delete m_world;
		m_world = nullptr;
	}
}

Actor* World::SpawnActor( ActorSpawnInfo actorSpawnInfo /*= ActorSpawnInfo()*/ )
{
	DEBUG_LOG( LOG::INFO, "Spawning actor in world..." );

	Actor* actor = new Actor();
	auto entities = m_world->CreateEntities( 1 );
	if( entities.size() == 1 )
	{
		actor->m_entity = entities[0];
		actor->m_world = this;
		if( actor->OnCreate( actorSpawnInfo ) == true )
		{
			if( m_worldParentActor != nullptr )
			{
				m_worldParentActor->AddChild( actor );
			}
			DEBUG_LOG( LOG::INFO, "Spawned actor!" );
			return actor;
		}
	}

	// clean up actor object, since we failed the spawn
	if( actor != nullptr )
	{
		actor->OnDestroy();
		delete actor;
		actor = nullptr;
	}

	DEBUG_LOG( LOG::WARNING, "Failed to spawn actor!" );

	return nullptr;
}

bool World::OnCreate()
{
	Actor* worldParentActor = SpawnActor( ActorSpawnInfo() );
	if( worldParentActor == nullptr )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Failed to create world: failed to create world parent actor" );
		return false;
	}
	
	m_worldParentActor = worldParentActor;
	m_worldParentActor->m_parent = nullptr;

	return true;
}

void World::OnDestroy()
{
	if( m_worldParentActor != nullptr )
	{
		m_worldParentActor->OnDestroy();	// calls on destroy for all child actors
	}	
}

void World::Update( const float deltaTime )
{
	if( m_worldParentActor != nullptr )
	{
		m_worldParentActor->Update( deltaTime );
	}
	if( m_world != nullptr )
	{
		m_world->Update( deltaTime );
	}
}