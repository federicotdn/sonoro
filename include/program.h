#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include <vector>
#include <string>

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

		static Program *loadFrom(std::string vertPath, std::string fragPath);

		int load();
		std::string getError() { return m_error; }
		bool isReady() { return m_object != 0; }
		GLuint getObject() { return m_object; }
		
		GLint attributeLocation(std::string name);
		GLint uniformLocation(std::string name);
		bool hasUniform(std::string name);

		void setUniformMatrix4fv(std::string name, const glm::mat4 &mat);
		void setUniform4fv(std::string name, const glm::vec4 &vec);
		void setUniform1i(std::string name, GLint val);

	private:
		std::vector<ShaderInfo> m_shaderList;
		GLuint m_object;
		std::string m_error;
	};
}

#endif /* PROGRAM_H */
