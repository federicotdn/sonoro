#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

namespace so
{
	class RenderContext
	{
	public:
		RenderContext();
		~RenderContext();
		int initialize(int win_width, int win_height);
		std::string getError() { return m_error; }
		SDL_Renderer *getRenderer() { return m_renderer; }
		int getWindowHeight();
		int getWindowWidth();
	
	private:
		SDL_Window *m_window;
		SDL_Renderer *m_renderer;
		std::string m_error;
		bool m_initialized;
	};
}

#endif /* RENDER_CONTEXT_H */
