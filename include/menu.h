#ifndef MENU_H
#define MENU_H

#include <constants.h>
#include <scene.h>
#include <text_renderer.h>

#include <map>

namespace so
{
	class Menu : public Scene
	{
	public:
		Menu(Sonoro &app);
		~Menu();

		int initialize() override;
		void draw() override;
		void update() override;

		std::string sceneName() { return "Menu"; }

		void showInputDevices();

	private:
		void reset();

		TextRenderer m_info;
		int m_currentSelection;
		std::map<int, int> m_menuIndexToPAIndex;
	};
}

#endif /* MENU_H */