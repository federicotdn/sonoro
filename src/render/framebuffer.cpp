#include <framebuffer.h>

using namespace so;

Framebuffer::Framebuffer() :
	m_object(0),
	m_texture(0),
	m_depthRbo(0),
	m_ready(false)
{
}

Framebuffer::~Framebuffer()
{
	if (m_depthRbo != 0)
	{
		glDeleteRenderbuffers(1, &m_depthRbo);
	}

	if (m_texture != 0)
	{
		glDeleteTextures(1, &m_texture);
	}

	if (m_object != 0)
	{
		glDeleteFramebuffers(1, &m_object);
	}
}

void Framebuffer::initialize(int width, int height)
{
	if (m_ready)
	{
		return;
	}

	glGenFramebuffers(1, &m_object);
	if (m_object == 0)
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_object);

	glGenTextures(1, &m_texture);
	if (m_texture == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

	glGenRenderbuffers(1, &m_depthRbo);
	if (m_depthRbo == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		m_ready = true;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
