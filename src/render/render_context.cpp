#include <render_context.h>
#include <constants.h>

using namespace so;

RenderContext::RenderContext() :
	m_window(nullptr),
	m_renderer(nullptr),
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

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		m_error = "Unable to initialize SDL.";
		return -1;
	}

	m_window = SDL_CreateWindow(APPLICATION_NAME, 100, 100, win_width, win_height, SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		m_error = "Unable to create SDL window.";
		SDL_Quit();
		return -1;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr)
	{
		m_error = "Unable to create SDL renderer.";
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return -1;
	}

	/* Initialize SDL_TTF */
	if (TTF_Init())
	{
		m_error = "Unable to initialize SDL_TTF";
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return -1;
	}

	m_initialized = true;
	return 0;
}

void RenderContext::getWindowSize(int * w, int * h)
{
	SDL_GetWindowSize(m_window, w, h);
}
