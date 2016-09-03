#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>

#define MAX_VERT_ANGLE 85.0f

using namespace so;

Camera::Camera() :
	m_projection(),
	m_horizontalAngle(0),
	m_verticalAngle(0),
	m_dirty(true),
	m_position()
{
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

void Camera::setProjection(glm::mat4 proj)
{
	m_projection = proj;
	m_dirty = true;
}

glm::vec3 Camera::getUpVector()
{
	glm::vec4 up = glm::inverse(getOrientationMatrix()) * glm::vec4(0.0f, 0.1f, 0.0f, 1.0f);
	return glm::vec3(up);
}

glm::vec3 Camera::getForwardVector()
{
	glm::vec4 forward = glm::inverse(getOrientationMatrix()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	return glm::vec3(forward);
}

glm::vec3 Camera::getRightVector()
{
	glm::vec4 forward = glm::inverse(getOrientationMatrix()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	return glm::vec3(forward);
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