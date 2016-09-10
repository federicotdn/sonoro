#include <lines.h>
#include <cassert>

#define UNIFORM_THICKNESS_NAME "u_thickness"

using namespace so;

Lines *Lines::createLines(Program &program, size_t points)
{
	GLint thicknessLocation = program.uniformLocation(UNIFORM_THICKNESS_NAME);
	if (thicknessLocation == -1)
	{
		return nullptr;
	}

	Model *model = Model::emptyModel(program, points * 2 * sizeof(glm::vec3), false);
	if (model == nullptr)
	{
		return nullptr;
	}

	model->m_drawType = GL_TRIANGLE_STRIP;

	Lines *lines = new Lines(model, points);

	return lines;
}

Lines::~Lines()
{
	delete m_model;
}

void Lines::setPoints(glm::vec3 points[], size_t count)
{
	assert(count == m_count);
}
