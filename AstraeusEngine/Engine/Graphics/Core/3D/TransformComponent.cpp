#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	ECS::Component( ID ),
	m_position( glm::vec3( 0.0f ) ),
	m_rotation( glm::vec3( 0.0f, 1.0f, 0.0f ) ),
	m_angle( 0.0f ),
	m_scale( glm::vec3( 1.0f, 1.0f, 1.0f ) )
{
	glm::mat4 model = glm::mat4( 1.0f );
	model = glm::translate( model, m_position );
	model = glm::rotate( model, m_angle, m_rotation );
	model = glm::scale( model, m_scale );
	m_transform = model;
}

TransformComponent::TransformComponent( glm::vec3 position, float angle, glm::vec3 rotation, glm::vec3 scale ) :
	ECS::Component( ID ),
	m_position( position ),
	m_angle( angle ),
	m_rotation( rotation ),
	m_scale( scale )
{
	glm::mat4 model = glm::mat4( 1.0f );
	model = glm::translate( model, m_position );
	model = glm::rotate( model, m_angle, m_rotation );
	model = glm::scale( model, m_scale );
	m_transform = model;
}

TransformComponent::~TransformComponent()
{}

glm::vec3 TransformComponent::GetPosition() const
{
	return m_position;
}

glm::vec3 TransformComponent::GetRotation() const
{
	return TransformComponent::m_rotation;
}

glm::vec3 TransformComponent::GetScale() const
{
	return m_scale;
}

glm::mat4 TransformComponent::GetTransform() const
{
	return m_transform;
}