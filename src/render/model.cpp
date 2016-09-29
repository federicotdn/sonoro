#include <model.h>
#include <asset_loader.h>
#include <cassert>

#define OBJ_VERTEX_IN_ATTRIB "i_vert"
#define OBJ_NORMAL_IN_ATTRIB "i_normal"
#define OBJ_UV_IN_ATTRIB "i_vertTexCoord"

using namespace so;

Model *Model::fromOBJFile(std::string path, Program &program, bool useStatic)
{
	GLint vertexLocation = program.attributeLocation(OBJ_VERTEX_IN_ATTRIB);
//	GLint normalLocation = program.attributeLocation(OBJ_NORMAL_IN_ATTRIB);
//	GLint uvLocation = program.attributeLocation(OBJ_UV_IN_ATTRIB);

	if (vertexLocation == -1)
	{
		return nullptr;
	}

	Asset3DModel modelInfo;
	bool ok = AssetLoader::getInstance().loadOBJFile(path, modelInfo);
	if (!ok)
	{
		return nullptr;
	}

	Model *model = new Model();
	model->m_program = &program;

	GLuint vertexVbo;//, normalVbo, uvVbo;
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	model->m_vao = vao;

	// Load vertex positions

	glGenBuffers(1, &vertexVbo);
	model->m_vertexVbo = vertexVbo;
	model->m_vbos.push_back(vertexVbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
	
	GLenum usage = useStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ARRAY_BUFFER, modelInfo.vertices.size() * sizeof(glm::vec3), &(modelInfo.vertices[0]), usage);
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load vertex normals

	// Load vertex uvs

	glBindVertexArray(0);

	model->m_drawType = GL_TRIANGLES;
	model->m_drawCount = (GLint)modelInfo.vertices.size();
	model->m_drawStart = 0;

	return model;
}

Model *Model::emptyModel(Program &program, size_t capacity, bool useStatic)
{
	GLint vertexLocation = program.attributeLocation(OBJ_VERTEX_IN_ATTRIB);
	GLint uvLocation = program.attributeLocation(OBJ_UV_IN_ATTRIB);

	if (vertexLocation == -1)
	{
		return nullptr;
	}

	GLenum usage = useStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

	Model *model = new Model();
	model->m_program = &program;

	GLuint vertexVbo;
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	model->m_vao = vao;

	// Load vertex positions

	glGenBuffers(1, &vertexVbo);
	model->m_vertexVbo = vertexVbo;
	model->m_vbos.push_back(vertexVbo);

	glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);

	glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(glm::vec3), nullptr, usage);
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (uvLocation != -1)
	{
		GLuint uvVbo;
		glGenBuffers(1, &uvVbo);
		model->m_vbos.push_back(uvVbo);
		model->m_uvVbo = uvVbo;

		glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
		
		glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(glm::vec2), nullptr, usage);
		glEnableVertexAttribArray(uvLocation);
		glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);

	model->m_drawType = GL_TRIANGLES;
	model->m_drawCount = (GLint)capacity;
	model->m_drawStart = 0;
	model->m_usage = usage;

	return model;
}

Model::~Model()
{
	glDeleteVertexArrays(1, &m_vao);
	for (auto it = m_vbos.begin(); it != m_vbos.end(); it++)
	{
		GLuint tmp = *it;
		glDeleteBuffers(1, (const GLuint*)&tmp);
	}
}

void Model::bufferVertexData(const void *data, size_t size)
{
	assert((GLint)size == m_drawCount);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec3), data, m_usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::bufferUvData(const void *data, size_t size)
{
	assert((GLint)size == m_drawCount);

	glBindBuffer(GL_ARRAY_BUFFER, m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec2), data, m_usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::bindVao()
{
	if (m_vao != 0)
	{
		glBindVertexArray(m_vao);
	}
}

void Model::unbindVao()
{
	glBindVertexArray(0);
}
