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
	//m_cam.setProjection(glm::perspective(glm::radians(90.0f), 4.0f / 4, 0.0f, 100.0f));
	m_cam.setProjection(glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -5.0f, 50.0f));
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
	InputContext &i = m_app.getInputContext();
	if (i.actionHeld(SonoroAction::UP))
	{
		m_cam.translate(m_cam.getForwardVector() * 0.1f);
	}
	else if (i.actionHeld(SonoroAction::DOWN))
	{
		m_cam.translate(-m_cam.getForwardVector() * 0.1f);
	}
	
	if (i.actionHeld(SonoroAction::RIGHT))
	{
		m_cam.translate(m_cam.getRightVector() * 0.1f);
	}
	else if (i.actionHeld(SonoroAction::LEFT))
	{
		m_cam.translate(-m_cam.getRightVector() * 0.1f);
	}

	const float mouseSensitivity = 0.1;
	double mouseX, mouseY;

	glfwGetCursorPos(m_app.getRenderContext().getWindow(), &mouseX, &mouseY);

	m_cam.rotate(mouseSensitivity * mouseY, mouseSensitivity * mouseX);

	glfwSetCursorPos(m_app.getRenderContext().getWindow(), 0, 0);
}

void TestScene::draw()
{
	Model *model = m_obj.getModel();
	Program *shader = model->m_shader;

	m_app.getRenderContext().useProgram(*shader);

	model->bindVao();

	shader->setUniformMatrix4fv("u_camera", m_cam.getViewMatrix());

	m_app.getRenderContext().drawArrays(model->m_drawType, model->m_drawStart, model->m_drawCount);

	model->unbindVao();

	m_app.getRenderContext().stopProgram();
}
