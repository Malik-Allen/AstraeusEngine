#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	explicit Camera( const int windowWidth, const int windowHeight );
	~Camera();

	glm::mat4 GetView() const;
	glm::mat4 GetPerspective() const;
	glm::mat4 GetOrthographic() const;
	glm::vec3 GetCameraPosition() const;
	glm::vec2 GetClippingPlanes() const;

	void SetPosition( glm::vec3 position );
	void SetRotation( const float yaw, const float pitch );
	void Zoom( const float y );

private:

	glm::vec3			m_position;
	glm::mat4			m_perspective;
	glm::mat4			m_orthographic;
	float				m_fieldOfView;
	float				m_yaw;
	float				m_pitch;
	float				m_nearPlane;
	float				m_farPlane;
	glm::vec3			m_forward;
	glm::vec3			m_up;
	glm::vec3			m_right;
	glm::vec3			m_worldUp;

	void Update();

};

#endif // !CAMERA_H

