#include <model.h>
#include <asset_loader.h>

#define OBJ_VERTEX_IN_ATTRIB "i_vert"
#define OBJ_NORMAL_IN_ATTRIB "i_normal"
#define OBJ_UV_IN_ATTRIB "i_uv"

using namespace so;

Model *Model::fromOBJFile(std::string path, Program &shader, bool useStatic)
{
	GLint vertexLocation = shader.attributeLocation(OBJ_VERTEX_IN_ATTRIB);
//	GLint normalLocation = shader.attributeLocation(OBJ_NORMAL_IN_ATTRIB);
//	GLint uvLocation = shader.attributeLocation(OBJ_UV_IN_ATTRIB);

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
	model->m_shader = &shader;

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
	glBufferData(GL_ARRAY_BUFFER, modelInfo.m_vertices.size() * sizeof(glm::vec3), &(modelInfo.m_vertices[0]), usage);
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load vertex normals

	// Load vertex uvs

	glBindVertexArray(0);

	model->m_drawType = GL_TRIANGLES;
	model->m_drawCount = (GLint)modelInfo.m_vertices.size();
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
