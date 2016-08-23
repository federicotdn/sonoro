#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>

#define MAX_VERT_ANGLE 85.0f

using namespace so;

Camera::Camera(glm::mat4 projection) :
	m_projection(projection),
	m_horizontalAngle(0),
	m_verticalAngle(0),
	m_dirty(true)
{
}

Camera::Camera(Camera& other, glm::mat4 projection) :
	Camera(projection)
{
	m_position = other.m_position;
	m_verticalAngle = other.m_verticalAngle;
	m_horizontalAngle = other.m_horizontalAngle;
}

Camera::~Camera()
{
}

void Camera::translate(glm::vec3 direction)
{
	m_position += direction;
	m_dirty = true;
}

void Camera::rotate(float horizontal, float vertical)
{
	m_horizontalAngle += horizontal;

	while (m_horizontalAngle > 360.0f)
	{
		m_horizontalAngle -= 360.0f;
	}

	while (m_horizontalAngle < 0.0f)
	{
		m_horizontalAngle += 360.0f;
	}

	m_verticalAngle += vertical;

	if (m_verticalAngle > MAX_VERT_ANGLE)
	{
		m_verticalAngle = MAX_VERT_ANGLE;
	}

	if (m_verticalAngle < -MAX_VERT_ANGLE)
	{
		m_verticalAngle = -MAX_VERT_ANGLE;
	}

	m_dirty = true;
}

glm::mat4 Camera::getOrientationMatrix()
{
	glm::mat4 orientation;
	orientation = glm::rotate(orientation, m_verticalAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	orientation = glm::rotate(orientation, m_horizontalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	return orientation;
}

glm::mat4 Camera::getViewMatrix()
{
	if (m_dirty)
	{
		m_prevViewMat = m_projection;
		m_prevViewMat *= getOrientationMatrix();
		m_prevViewMat = glm::translate(m_prevViewMat, -m_position);

		m_dirty = false;
	}

	return m_prevViewMat;
}