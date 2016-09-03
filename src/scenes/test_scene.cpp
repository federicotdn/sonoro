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
	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();

	m_cam.setProjection(glm::perspective(glm::radians(60.0f), w / h, 0.0f, 100.0f));
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

#include <iostream>

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

	const float mouseSensitivity = 0.01f;
	double mouseX, mouseY;

	glfwGetCursorPos(m_app.getRenderContext().getWindow(), &mouseX, &mouseY);

	m_cam.rotate(mouseSensitivity * (float)mouseX, mouseSensitivity * (float)mouseY);

	glfwSetCursorPos(m_app.getRenderContext().getWindow(), 0, 0);
}

void TestScene::draw()
{
	Model *model = m_obj.getModel();
	Program *shader = model->m_program;

	m_app.getRenderContext().useProgram(*shader);

	model->bindVao();

	float ticks = (float)m_app.getRenderContext().getTicks() / 100;
	float r = sinf(ticks + (PI_FLOAT / 3.2f));
	float g = sinf(ticks + (PI_FLOAT / 4.1f));
	float b = sinf(ticks + PI_FLOAT);
	glm::vec4 color(r, g, b, 1);

	m_program->setUniform4fv("u_inColor", color);
	shader->setUniformMatrix4fv("u_camera", m_cam.getViewMatrix());

	m_app.getRenderContext().drawArrays(model->m_drawType, model->m_drawStart, model->m_drawCount);

	model->unbindVao();

	m_app.getRenderContext().stopProgram();
}
