#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include <glm/glm.hpp>
#include <model.h>

namespace so
{
	class WorldObject
	{
	public:
		WorldObject();
		~WorldObject();

	private:
		glm::mat4 m_transform;
		Model *m_model;
	};
}

#endif /* WORLD_OBJECT_H */