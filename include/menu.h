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
		void updateCircle(bool initial);

		TextRenderer m_info;
		int m_currentSelection;
		std::map<int, int> m_menuIndexToPAIndex;
		SDL_Point m_points[AC_OUT_SIZE];
	};
}

#endif /* MENU_H */