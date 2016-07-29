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
		struct MenuResult {
			int inputDeviceIndex = 0;
		};

		Menu(RenderContext &renderContext, AudioContext &audioContext, KeyboardMap &keyboard, MenuResult &result);
		~Menu();

		int initialize() override;
		void reset();
		void draw() override;
		int update() override;

		void showInputDevices();

	private:
		int getKeyboardNum();

		MenuResult &m_menuResult;
		TextRenderer m_info;
	};
}

#endif /* MENU_H */