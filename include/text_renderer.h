#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>

#include <render_context.h>

namespace so
{
	class TextRenderer
	{
	public:
		TextRenderer();
		~TextRenderer();
		int initialize(std::string fontPath, int size, int wrapSize);
		SDL_Texture *getTexture(RenderContext &context);
		SDL_Rect *getRect() { return &m_quad; }
		
		void setPosition(int x, int y);
		void setFontSize(int size);
		void append(std::string text);
		void appendln(std::string text);
		void setText(std::string text);
		void clear();
		void setColor(SDL_Color color);
		void setBackgroundColor(SDL_Color color);

	private:
		bool m_dirty;

		std::string m_text;
		int m_fontSize;
		int m_wrapSize;
		SDL_Rect m_quad;
		TTF_Font *m_font;
		SDL_Texture *m_texture;
		SDL_Color m_color;
		SDL_Color *m_backgroundColor;
	};
}

#endif /* TEXT_RENDERER_H */
