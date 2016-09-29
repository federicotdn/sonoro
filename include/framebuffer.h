#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

namespace so
{
	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();

		void initialize(int width, int height);
		bool ready() { return m_ready; }

		GLuint getTexture() { return m_texture; }
		GLuint getFramebuffer() { return m_object; }

	private:
		GLuint m_object;
		GLuint m_texture;
		GLuint m_depthRbo;
		bool m_ready;
	};
}

#endif /* FRAMEBUFFER_H */