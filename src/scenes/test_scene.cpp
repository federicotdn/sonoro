#include <test_scene.h>

#include <vector>
#include <iostream>

using namespace so;

TestScene::TestScene(Sonoro &app) :
	Scene(app),
	m_program(nullptr)
{
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

	return 0;
}

void TestScene::update()
{
}

void TestScene::draw()
{
	if (m_program == nullptr)
	{
		return;
	}

	m_app.getRenderContext().useProgram(*m_program);
}
