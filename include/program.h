#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>

#include <vector>

namespace so
{
	class Program
	{
	public:
		struct ShaderInfo {
			std::string fileName;
			GLenum shaderType;
		};

		Program(std::vector<ShaderInfo> shaderList);
		~Program();

		int load();
		std::string getError() { return m_error; }
		bool isReady() { return m_object != -1; }
		GLuint getObject() { return m_object; }

	private:
		std::vector<ShaderInfo> m_shaderList;
		GLuint m_object;
		std::string m_error;
	};
}

#endif /* PROGRAM_H */