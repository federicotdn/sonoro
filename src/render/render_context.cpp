#include <render_context.h>
#include <constants.h>

#include <sstream>

using namespace so;

RenderContext::RenderContext() :
	m_window(nullptr),
	m_initialized(false),
	m_fullscreen(false),
	m_activeProgram(0),
	m_deltaMs(0)
{
}

RenderContext::~RenderContext()
{
	if (!m_initialized)
	{
		return;
	}

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

int RenderContext::initialize(int win_width, int win_height)
{
	if (m_initialized)
	{
		m_error = "RenderContext is already initialized.";
		return -1;
	}

	if (!glfwInit())
	{
		m_error = "Unable to initialize GLFW.";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(win_width, win_height, APPLICATION_NAME, nullptr, nullptr);
	if (!m_window)
	{
		m_error = "Unable to create GLFW window.";
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(m_window, 0, 0);

	glewExperimental = true;
	int status = glewInit();
	if (status != GLEW_OK || !GLEW_VERSION_3_2)
	{
		const char *er = reinterpret_cast<const char*>(glewGetErrorString(status));
		m_error = "Unable to initialize GLEW: " + std::string(er);
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_initialized = true;
	return 0;
}

std::string RenderContext::getGLInfoString()
{
	std::ostringstream os;
    os << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    os << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    os << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    os << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    return os.str();
}

int RenderContext::getWindowHeight()
{
	return 800;
}

int RenderContext::getWindowWidth()
{
	return 600;
}

void RenderContext::setFullscreen(bool enabled)
{
	//SDL_SetWindowFullscreen(m_window, enabled ? SDL_WINDOW_FULLSCREEN : 0);
	m_fullscreen = enabled;
}

void RenderContext::useProgram(Program &program)
{
	GLuint obj = program.getObject();
	if (m_activeProgram != obj)
	{
		glUseProgram(obj);
	}
}

void RenderContext::stopProgram()
{
	glUseProgram(0);
}

void RenderContext::drawArrays(GLenum mode, GLint first, GLsizei count)
{
	glDrawArrays(mode, first, count);
}
