#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace so
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void translate(glm::vec3 direction);
		void rotate(float horizontal, float vertical);
		void setProjection(glm::mat4 proj);
		void setFullscreenProjection(int w, int h);

		glm::vec3 getUpVector();
		glm::vec3 getForwardVector();
		glm::vec3 getRightVector();

		glm::mat4 getViewMatrix();

	private:
		glm::mat4 getOrientationMatrix();

		float m_horizontalAngle;
		float m_verticalAngle;
		glm::vec3 m_position;
		glm::mat4 m_projection;

		bool m_dirty;
		glm::mat4 m_prevViewMat;
	};
}

#endif /* CAMERA_H */