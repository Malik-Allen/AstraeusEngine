#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera( const int windowWidth, const int windowHeight )
{
	m_position = glm::vec3( 0.0f, 0.0f, 25.0f );
	m_fieldOfView = 45.0f;
	m_forward = glm::vec3( 0.0f, 0.0f, -1.0f );
	m_up = glm::vec3( 0.0f, 1.0f, 0.0f );
	m_worldUp = m_up;
	m_nearPlane = 0.5f;
	m_farPlane = 100.0f;
	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_right = glm::vec3();

	float aspect = static_cast< float >( windowWidth ) / static_cast< float >( windowHeight );

	m_perspective = glm::perspective(
		m_fieldOfView,
		aspect,
		m_nearPlane,
		m_farPlane
	);

	m_orthographic = glm::ortho(
		0.0f,
		static_cast< float >( windowWidth ),
		0.0f,
		static_cast< float >( windowHeight ),
		-1.0f,
		-1.0f
	);
}

Camera::~Camera()
{}

glm::mat4 Camera::GetView() const
{
	return glm::lookAt( m_position, m_position + m_forward, m_up );
}
glm::mat4 Camera::GetPerspective() const
{
	return m_perspective;
}
glm::mat4 Camera::GetOrthographic() const
{
	return m_orthographic;
}
glm::vec3 Camera::GetCameraPosition() const
{
	return m_position;
}
glm::vec2 Camera::GetClippingPlanes() const
{
	return glm::vec2( m_nearPlane, m_farPlane );
}

void Camera::SetPosition( glm::vec3 position )
{
	m_position = position;
	Update();
}

void Camera::SetRotation( const float yaw, const float pitch )
{
	m_yaw = yaw;
	m_pitch = pitch;
	Update();
}

void Camera::Zoom( const float y )
{
	if( y < 0 || y > 0 )
	{
		m_position += y * ( m_forward * 2.0f );
	}
	Update();
}

void Camera::Update()
{
	m_forward.x = cos( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
	m_forward.y = sin( glm::radians( m_pitch ) );
	m_forward.z = sin( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );

	m_forward = glm::normalize( m_forward );

	m_right = glm::normalize( glm::cross( m_forward, m_worldUp ) );

	m_up = glm::normalize( glm::cross( m_right, m_forward ) );
}
