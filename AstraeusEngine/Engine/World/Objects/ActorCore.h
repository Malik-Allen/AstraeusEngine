#ifndef ACTORCORE_H
#define ACTORCORE_H


#include <glm/glm.hpp>

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

#endif // !ACTORCORE_H

