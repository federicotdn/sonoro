#include <render_context.h>
#include <constants.h>

#include <sstream>

using namespace so;

RenderContext::RenderContext() :
	m_window(nullptr),
	m_initialized(false),
	m_fullscreen(false),
	m_activeProgram(0),
	m_deltaMs(0),
	m_clearMask(0)
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

int RenderContext::initialize(int win_width, int win_height, bool fullscreen)
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

	// Use OpenGL 3.2, GLSL #version 150
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = nullptr;
	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
	}
	
	m_window = glfwCreateWindow(win_width, win_height, APPLICATION_NAME, monitor, nullptr);
	if (!m_window)
	{
		m_error = "Unable to create GLFW window.";
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
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
	glEnable(GL_BLEND);
	glClearColor(0, 0, 0, 1);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	enableClearBit(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

uint32_t RenderContext::getTicks()
{
	return (uint32_t)(glfwGetTime() * 1000);
}

void RenderContext::enableClearBit(GLbitfield bits)
{
	m_clearMask = m_clearMask | bits;
}

void RenderContext::disableClearBit(GLbitfield bits)
{
	m_clearMask = m_clearMask & (~bits);
}

void RenderContext::clearScreen(GLbitfield bits)
{
	bits = bits & m_clearMask;
	glClear(bits);
}

void RenderContext::bindFramebuffer(GLuint fb)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void RenderContext::bindDefaultFramebuffer()
{
	bindFramebuffer(0);
}

void RenderContext::bindTexture(GLenum unit, GLuint texture)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(0);
}

int RenderContext::getWindowHeight()
{
	int h;
	glfwGetFramebufferSize(m_window, nullptr, &h);
	return h;
}

int RenderContext::getWindowWidth()
{
	int w;
	glfwGetFramebufferSize(m_window, &w, nullptr);
	return w;
}

void RenderContext::setFullscreen(bool enabled)
{
	//TODO: Immplement
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
