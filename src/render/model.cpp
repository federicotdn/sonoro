#include <model.h>
#include <asset_loader.h>

#define OBJ_VERTEX_IN_ATTRIB "i_vert"
#define OBJ_NORMAL_IN_ATTRIB "i_normal"
#define OBJ_UV_IN_ATTRIB "i_uv"

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
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load vertex normals

	// Load vertex uvs

	glBindVertexArray(0);

	model->m_drawType = GL_TRIANGLES;
	model->m_drawCount = (GLint)modelInfo.vertices.size();
	model->m_drawStart = 0;

	return model;
}

Model *Model::emptyModel(Program & program, size_t capacity, bool useStatic)
{
	GLint vertexLocation = program.attributeLocation(OBJ_VERTEX_IN_ATTRIB);

	if (vertexLocation == -1)
	{
		return nullptr;
	}

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

	GLenum usage = useStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, usage);
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	model->m_drawType = GL_TRIANGLES;
	model->m_drawCount = (GLint)capacity;
	model->m_drawStart = 0;

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
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
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
