#include <lines.h>
#include <cassert>

#include <glm/glm.hpp>

using namespace so;

Lines *Lines::createLines(Program &program, size_t points)
{
	assert(points > 1);

	Model *model = Model::emptyModel(program, points * 2, false);
	if (model == nullptr)
	{
		return nullptr;
	}

	model->m_drawType = GL_TRIANGLE_STRIP;

	Lines *lines = new Lines(model, points);

	return lines;
}

Lines::Lines(Model *model, size_t count) :
	m_model(model),
	m_count(count),
	m_thickness(1.0f)
{
	m_pointsBuf.resize(count * 2);
}

Lines::~Lines()
{
	delete m_model;
}

void Lines::setPoints(glm::vec3 points[], size_t count)
{
	assert(count == m_count);
	for (int i = 0; i < count; i++)
	{
		glm::vec3 offset(m_thickness / 2, 0, 0);

		m_pointsBuf[(i * 2)] = points[i] + offset;
		m_pointsBuf[(i * 2) + 1] = points[i] - offset;
	}

	m_model->bufferVertexData(&(m_pointsBuf[0]), count * 2);
}

//void Lines::setPoints(glm::vec3 points[], size_t count)
//{
//	assert(count == m_count);
//
//	//First point special case
//	{
//		glm::vec3 dir = points[1] - points[0];
//		dir.z = 0;
//		glm::vec3 perpendicular = glm::normalize(glm::vec3(-dir.y, dir.x, 0)) * (m_thickness / 2);
//		float sign = glm::sign(perpendicular.y);
//
//		m_pointsBuf[0] = points[0] + perpendicular * sign;
//		m_pointsBuf[1] = points[0] - perpendicular * sign;
//	}
//
//	for (int i = 1; i < count - 1; i++)
//	{
//		glm::vec3 dir1 = points[i - 1] - points[i];
//		glm::vec3 dir2 = points[i + 1] - points[i];
//
//		dir1.z = dir2.z = 0;
//
//		float len1 = dir1.length();
//		float len2 = dir2.length();
//
//		glm::vec3 bisector = glm::normalize((dir1 * len2) + (dir2 * len1)) * (m_thickness / 2);
//		
//		float sign = glm::sign(bisector.y);
//
//		m_pointsBuf[(i * 2)] = points[i] + bisector * sign;
//		m_pointsBuf[(i * 2) + 1] = points[i] - bisector * sign;
//	}
//
//	//Last point special case
//	{
//		glm::vec3 dir = points[count - 2] - points[count - 1];
//		dir.z = 0;
//		glm::vec3 perpendicular = glm::normalize(glm::vec3(-dir.y, dir.x, 0));
//		float sign = glm::sign(perpendicular.y);
//
//		m_pointsBuf[(count * 2) - 2] = points[count - 1] + perpendicular * sign;
//		m_pointsBuf[(count * 2) - 1] = points[count - 1] - perpendicular * sign;
//	}
//
//	m_model->bufferVertexData(&(m_pointsBuf[0]), count * 2);
//}
