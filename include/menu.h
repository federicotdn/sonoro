#ifndef MENU_H
#define MENU_H

#include <constants.h>
#include <scene.h>
#include <shapes.h>

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

		std::string sceneName() override { return "Menu"; }

		void showInputDevices();

	private:
		void reset();
		void updateCircle(bool initial);

		int m_currentSelection;
		std::map<int, int> m_menuIndexToPAIndex;
		Point m_points[AC_OUT_SIZE];
	};
}

#endif /* MENU_H */