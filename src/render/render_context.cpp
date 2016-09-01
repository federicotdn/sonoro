#include <render_context.h>
#include <constants.h>

using namespace so;

RenderContext::RenderContext() :
	m_window(nullptr),
	m_renderer(nullptr),
	m_glContext(nullptr),
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

	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int RenderContext::initialize(int win_width, int win_height)
{
	if (m_initialized)
	{
		m_error = "RenderContext is already initialized.";
		return -1;
	}

	/* Initialize SDL */

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		m_error = "Unable to initialize SDL.";
		return -1;
	}

	m_window = SDL_CreateWindow(APPLICATION_NAME, 100, 100, win_width, win_height, SDL_WINDOW_OPENGL);
	if (m_window == nullptr)
	{
		m_error = "Unable to create SDL window.";
		SDL_Quit();
		return -1;
	}

	m_glContext = SDL_GL_CreateContext(m_window);

	if (m_glContext == nullptr)
	{
		m_error = "Unable to create GL context.";
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK || !GLEW_VERSION_3_2)
	{
		m_error = "Unable to initialize GLEW.";
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr)
	{
		m_error = "Unable to create SDL renderer.";
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return -1;
	}

	/* Initialize SDL_TTF */
	if (TTF_Init())
	{
		m_error = "Unable to initialize SDL_TTF";
		SDL_DestroyRenderer(m_renderer);
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return -1;
	}

	m_initialized = true;
	return 0;
}

int RenderContext::getWindowHeight()
{
	int h;
	SDL_GetWindowSize(m_window, nullptr, &h);
	return h;
}

int RenderContext::getWindowWidth()
{
	int w;
	SDL_GetWindowSize(m_window, &w, nullptr);
	return w;
}

void RenderContext::setFullscreen(bool enabled)
{
	SDL_SetWindowFullscreen(m_window, enabled ? SDL_WINDOW_FULLSCREEN : 0);
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
