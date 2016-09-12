#ifndef LINE_MODEL_H
#define LINE_MODEL_H

#include <model.h>
#include <glm/vec3.hpp>

namespace so
{
	class Lines
	{
	public:
		static Lines *createLines(Program &program, size_t points);
		~Lines();
	
		void setThickness(float thickness) { m_thickness = thickness; }
		void setPoints(glm::vec3 points[], size_t count);
		Model *getModel() { return m_model; }

	private:
		Lines(Model *model, size_t count);

		Model *m_model;
		size_t m_count;
		float m_thickness;
		std::vector<glm::vec3> m_pointsBuf;
	};
}

#endif /* LINE_MODEL_H */