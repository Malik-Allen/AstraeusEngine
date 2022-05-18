#ifndef ACTOR_H
#define ACTOR_H

#include "ActorCore.h"
#include "../World.h"

#include <ECS.h>
#include <vector>

class World;
class TransformComponent;



/*
*	The Actor class represents an object instantiated in the world, that has a transform component
*	Actor is responsible for maintaining it's relationships with it's children and parent
*	Components can be added to and removed from actors
*	Each Actor hold an entityId which represents their data inside of the ECS
*/
class Actor
{
	friend class World;
public:
	Actor();
	virtual ~Actor();

	/*
	*	Adds actor as child to this actor
	*/
	void AddChild( Actor* child );

	/*
	*	Returns the array of children actors to this actor
	*/
	const std::vector<Actor*>& GetChildActors() const;


	// Adds Component this actor
	template<typename T, typename ... Args>
	T* AddComponent( Args&& ... args )
	{
		return m_world->GetECS()->AddComponentToEntity<T, Args ...>( m_entity, std::forward<Args>( args ) ... );
	}

	// Finds component on this actor
	template<typename T>
	T* FindComponentByClass()
	{
		return m_world->GetECS()->FindComponentInEntity<T>( m_entity );
	}

	// Removes the first component found of the same type from this actor
	template<typename T>
	void RemoveComponent()
	{
		m_world->GetECS()->RemoveComponentFromEntity<T>( m_entity );
	}

private:
	ECS::EntityId m_entity;
	World* m_world;
	Actor* m_parent;
	std::vector<Actor*> m_children;

	/*
	*	Removes child actor from children vector on actor
	*/
	void RemoveChild( Actor* child );

	bool OnCreate( ActorSpawnInfo actorSpawnInfo = ActorSpawnInfo() );
	void OnDestroy();
	void Update( const float deltaTime );

	TransformComponent* m_transformComponent;

protected:
	virtual bool Actor_OnCreate( ActorSpawnInfo actorSpawnInfo );
	virtual void Actor_OnDestroy();
	virtual void Actor_Update( const float deltaTime );
};

#endif // !ACTOR_H