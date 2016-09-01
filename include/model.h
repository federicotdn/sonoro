#ifndef MODEL_H
#define MODEL_H

#include <program.h>
#include <vector>

namespace so
{
	class Model
	{
	public:
		static Model *fromOBJFile(std::string path, Program &shader, bool useStatic = true);
		~Model();
		
		Program *m_shader;
		GLuint m_vao;
		GLenum m_drawType;
		GLint m_drawCount;
		GLint m_drawStart;
		GLuint m_vertexVbo;

		void bindVao();
		void unbindVao();

	private:
		Model() :
			m_shader(nullptr),
			m_vao(0),
			m_drawType(GL_TRIANGLES),
			m_drawCount(0),
			m_drawStart(0),
			m_vertexVbo(0)
		{}

		std::vector<GLuint> m_vbos;
	};
}

#endif /* MODEL_H */