#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <string>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <program.h>

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
		void setFullscreen(bool enabled);
		bool getFullscreen() { return m_fullscreen; }

		void useProgram(Program &program);

	private:
		SDL_Window *m_window;
		SDL_Renderer *m_renderer;
		SDL_GLContext m_glContext;
		std::string m_error;
		bool m_initialized;
		bool m_fullscreen;
		GLuint m_activeProgram;
	};
}

#endif /* RENDER_CONTEXT_H */
