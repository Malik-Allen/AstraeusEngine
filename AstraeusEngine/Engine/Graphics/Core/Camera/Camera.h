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
	void SetRotation( glm::vec3 rotation );
	void Zoom( const float y );

	void Translate( glm::vec3 delta );
	void Rotate( glm::vec3 delta );

private:

	glm::vec3			m_position;
	// XYZ(Pitch, Roll, Yaw)
	glm::vec3			m_rotation;
	glm::mat4			m_perspective;
	glm::mat4			m_orthographic;
	float				m_fieldOfView;
	float				m_nearPlane;
	float				m_farPlane;
	glm::vec3			m_forward;
	glm::vec3			m_up;
	glm::vec3			m_right;
	glm::vec3			m_worldUp;

	float				m_movementSpeed;
	float				m_rotationSpeed;

	void Update();
	void Update( const float deltaTime );

};

#endif // !CAMERA_H

