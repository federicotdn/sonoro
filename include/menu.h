#ifndef MENU_H
#define MENU_H

#include <constants.h>
#include <scene.h>
#include <text_renderer.h>

namespace so
{
	class Menu : public Scene
	{
	public:
		Menu(RenderContext &renderContext, AudioContext &audioContext, KeyboardMap &keyboard);
		~Menu();

		int initialize() override;
		void reset();
		void draw() override;
		int update() override;

		void showInputDevices();

	private:
		TextRenderer m_info;
	};
}

#endif /* MENU_H */