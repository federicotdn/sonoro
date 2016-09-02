#include <test_scene.h>

#include <vector>
#include <iostream>	

#include <glm/gtc/matrix_transform.hpp>

using namespace so;

TestScene::TestScene(Sonoro &app) :
	Scene(app),
	m_program(nullptr),
	m_obj(nullptr)
{
	m_cam.setProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9, 0.0f, 100.0f));
}

TestScene::~TestScene()
{
	if (m_program != nullptr)
	{
		delete m_program;
	}
}

int TestScene::initialize()
{
	std::vector<Program::ShaderInfo> shaders =
	{
		{ "resources/default.vert", GL_VERTEX_SHADER },
		{ "resources/default.frag", GL_FRAGMENT_SHADER }
	};

	m_program = new Program(shaders);
	if (m_program->load())
	{
		std::cerr << "TestScene: error loading shader:" << std::endl;
		std::cerr << m_program->getError() << std::endl;
		return -1;
	}

	Model *model = Model::fromOBJFile("resources/cube.obj", *m_program);
	m_obj.setModel(model);

	return 0;
}

void TestScene::update()
{

}

#include <iostream>

void TestScene::draw()
{
	Model *model = m_obj.getModel();
	Program *shader = model->m_shader;

	m_app.getRenderContext().useProgram(*shader);

	model->bindVao();

	m_app.getRenderContext().drawArrays(model->m_drawType, model->m_drawStart, model->m_drawCount);

	model->unbindVao();

	m_app.getRenderContext().stopProgram();
}
