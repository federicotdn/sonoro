#include <text_renderer.h>

using namespace so;

TextRenderer::TextRenderer() :
	m_texture(nullptr),
	m_font(nullptr),
	m_quad{0, 0, 0, 0},
	m_fontSize(0),
	m_dirty(true),
	m_color{255, 255, 255},
	m_backgroundColor(nullptr)
{
}

TextRenderer::~TextRenderer()
{
	if (m_backgroundColor != nullptr)
	{
		delete m_backgroundColor;
	}

	if (m_font != nullptr)
	{
		TTF_CloseFont(m_font);
	}

	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
	}
}

int TextRenderer::initialize(std::string fontPath, int size, int wrapSize)
{
	m_font = TTF_OpenFont(fontPath.c_str(), size);
	if (m_font == nullptr)
	{
		return -1;
	}

	m_fontSize = size;
	m_wrapSize = wrapSize;

	return 0;
}

SDL_Texture *TextRenderer::getTexture(RenderContext &context)
{
	if (m_font == nullptr)
	{
		return nullptr;
	}

	if (!m_dirty)
	{
		return m_texture;
	}

	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
	}

	SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(m_font, m_text.c_str(), m_color, m_wrapSize);

	m_quad.w = textSurface->w;
	m_quad.h = textSurface->h;

	if (m_backgroundColor != nullptr)
	{
		SDL_Surface *background = SDL_CreateRGBSurface(0, m_quad.w, m_quad.h, 32, 0, 0, 0, 0);
		SDL_FillRect(background, NULL, SDL_MapRGB(background->format, m_backgroundColor->r, m_backgroundColor->g, m_backgroundColor->b));

		SDL_BlitSurface(textSurface, nullptr, background, nullptr);
		m_texture = SDL_CreateTextureFromSurface(context.getRenderer(), background);
		SDL_FreeSurface(background);
	}
	else
	{
		m_texture = SDL_CreateTextureFromSurface(context.getRenderer(), textSurface);
	}
	
	SDL_FreeSurface(textSurface);

	m_dirty = false;
	return m_texture;
}

void TextRenderer::setPosition(int x, int y)
{
	m_quad.x = x;
	m_quad.y = y;
}

void TextRenderer::append(std::string text)
{
	if (text.length() == 0)
	{
		return;
	}

	m_dirty = true;
	m_text.append(text);
}

void TextRenderer::appendln(std::string text)
{
	append(text + "\n");
}

void TextRenderer::setText(std::string text)
{
	clear();
	append(text);
}

void TextRenderer::clear()
{
	if (m_text.length() == 0)
	{
		return;
	}

	m_dirty = true;
	m_text.clear();
}

void TextRenderer::setFontSize(int size)
{
	if (size == m_fontSize)
	{
		return;
	}

	m_dirty = true;
	m_fontSize = size;
}

void TextRenderer::setColor(SDL_Color color)
{
	m_dirty = true;
	m_color = color;
}

void TextRenderer::setBackgroundColor(SDL_Color color)
{
	m_dirty = true;
	if (m_backgroundColor != nullptr)
	{
		delete m_backgroundColor;
	}

	m_backgroundColor = new SDL_Color(color);
}
