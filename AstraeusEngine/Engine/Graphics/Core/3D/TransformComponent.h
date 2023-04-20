// Copyright (c) 2022 Malik Allen

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <ECS.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

class TransformComponent : public ECS::Component
{
public:
	static constexpr uint64_t ID = GENERATE_ID( "TransformComponent" );

	TransformComponent();
	TransformComponent( glm::vec3 position, float angle, glm::vec3 rotation, glm::vec3 scale );
	~TransformComponent();

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	glm::mat4 GetTransform() const;
	

private:
	glm::vec3	m_position;
	// TODO: change orientation to quaternion
	float		m_angle;
	glm::vec3	m_rotation;
	glm::vec3	m_scale;
	glm::mat4	m_transform;

};

#endif // !TRANSFORMCOMPONENT_H

