#include <render_context.h>
#include <constants.h>

using namespace so;

RenderContext::RenderContext() :
	m_window(nullptr),
	m_renderer(nullptr),
	m_glContext(nullptr),
	m_initialized(false)
{
}

RenderContext::~RenderContext()
{
	if (!m_initialized)
	{
		return;
	}

	TTF_Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyRenderer(m_renderer);
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

	SDL_GLContext m_glContext = SDL_GL_CreateContext(m_window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (m_glContext == nullptr)
	{
		m_error = "Unable to create GL context.";
		SDL_Quit();
		return -1;
	}

	glewExperimental = true;
	if (glewInit() != GLEW_OK || !GLEW_VERSION_3_2)
	{
		m_error = "Unable to initialize GLEW.";
		SDL_Quit();
		return -1;
	}

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
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyRenderer(m_renderer);
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
