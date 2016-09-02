#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
		
		GLFWwindow *getWindow() { return m_window; }
		int getWindowHeight();
		int getWindowWidth();
		void setFullscreen(bool enabled);
		bool getFullscreen() { return m_fullscreen; }
		std::string getGLInfoString();

		void setDeltaMs(uint32_t ms) { m_deltaMs = ms; }
		uint32_t getDeltaMs() { return m_deltaMs; }

		void useProgram(Program &program);
		void stopProgram();

		void drawArrays(GLenum mode, GLint first, GLsizei count);

	private:
		GLFWwindow *m_window;
		std::string m_error;
		bool m_initialized;
		bool m_fullscreen;
		GLuint m_activeProgram;
		uint32_t m_deltaMs;
	};
}

#endif /* RENDER_CONTEXT_H */
