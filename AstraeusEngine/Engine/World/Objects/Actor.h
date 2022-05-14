#ifndef ACTOR_H
#define ACTOR_H

#include <ECS.h>

#include <vector>
#include <glm/glm.hpp>

class World;
class TransformComponent;

/*
*	Used to provide information about the spawn of an actor
*/
struct ActorSpawnInfo
{
	ActorSpawnInfo() :
		position( glm::vec3( 0.0f ) ),
		rotation( glm::vec3( 0.0f ) ),
		scale( glm::vec3( 1.0f ) )
	{};

	ActorSpawnInfo( glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale ) :
		position( _position ),
		rotation( _rotation ),
		scale( _scale )
	{};

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

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

private:
	ECS::EntityId m_entity;
	World* m_world;
	Actor* m_parent;
	std::vector<Actor*> m_children;

	/*
	*	Removes child actor from children vector on actor
	*/
	void RemoveChild( Actor* child );

	bool OnCreate( ActorSpawnInfo actorSpawnInfo );
	void OnDestroy();
	void Update( const float deltaTime );

	TransformComponent* m_transformComponent;

protected:
	virtual bool Actor_OnCreate( ActorSpawnInfo actorSpawnInfo );
	virtual void Actor_OnDestroy();
	virtual void Actor_Update( const float deltaTime );
};

#endif // !ACTOR_H

