#include <program.h>

#include <asset_loader.h>

using namespace so;

Program::Program(std::vector<ShaderInfo> shaderList) :
	m_shaderList(shaderList),
	m_object(0)
{
}

Program::~Program()
{
	if (isReady())
	{
		glDeleteProgram(m_object);
		m_object = 0;
	}
}

int Program::load()
{
	if (m_shaderList.size() == 0)
	{
		m_error = "Empty shader list.";
		return -1;
	}

	std::vector<GLuint> shaderObjects;
	shaderObjects.reserve(m_shaderList.size());
	GLint status;

	for (int i = 0; i < m_shaderList.size(); i++)
	{
		std::string fileName = m_shaderList[i].fileName;
		std::string shaderText;
		bool success = AssetLoader::getInstance().readTextFile(fileName, shaderText);
		if (!success)
		{
			m_error = std::string("Unable to read shader file: ") + fileName;

			for (int j = 0; j < i; j++)
			{
				glDeleteShader(shaderObjects[j]);
			}
			return -1;
		}

		const char *code = shaderText.c_str();
		shaderObjects.push_back(glCreateShader(m_shaderList[i].shaderType));
		if (shaderObjects[i] == 0)
		{
			m_error = std::string("Unable to create shader for file: ") + fileName;

			for (int j = 0; j < i; j++)
			{
				glDeleteShader(shaderObjects[j]);
			}
			return -1;
		}

		glShaderSource(shaderObjects[i], 1, (const GLchar**)&code, nullptr);
		glCompileShader(shaderObjects[i]);

		glGetShaderiv(shaderObjects[i], GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			std::string msg = "Failed to compile shader: ";
			msg += fileName + ":\n";
			
			GLint logLength;
			glGetShaderiv(shaderObjects[i], GL_INFO_LOG_LENGTH, &logLength);
			
			char *infoLog = new char[logLength + 1];
			glGetShaderInfoLog(shaderObjects[i], logLength, nullptr, infoLog);
			msg += infoLog;
			delete[] infoLog;

			m_error = msg;

			for (int j = 0; j < i + 1; j++)
			{
				glDeleteShader(shaderObjects[j]);
			}
			return -1;
		}
	} /* Done loading shaders */

	m_object = glCreateProgram();
	if (m_object == 0)
	{
		m_error = "Unable to create GL program.";
		for (int i = 0; i < shaderObjects.size(); i++)
		{
			glDeleteShader(shaderObjects[i]);
		}

		m_object = 0;
		return -1;
	}

	for (int i = 0; i < shaderObjects.size(); i++)
	{
		glAttachShader(m_object, shaderObjects[i]);
	}

	glLinkProgram(m_object);

	for (int i = 0; i < shaderObjects.size(); i++)
	{
		glDetachShader(m_object, shaderObjects[i]);
		glDeleteShader(shaderObjects[i]);
	}

	glGetProgramiv(m_object, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg = "Program link failure:\n";

		GLint logLength;
		glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &logLength);
		
		char *infoLog = new char[logLength + 1];
		glGetProgramInfoLog(m_object, logLength, nullptr, infoLog);
		msg += infoLog;
		delete[] infoLog;

		glDeleteProgram(m_object);
		m_object = 0;

		return -1;
	}

	return 0;
}
