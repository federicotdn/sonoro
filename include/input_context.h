#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <map>

namespace so
{
	using KeyboardMap = std::map<int, bool>;

	enum SonoroAction
	{
		NEXT_SCENE = 0,
		PREV_SCENE,
		SMOOTH_UP,
		SMOOTH_DOWN,
		TOGGLE_HUD,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SELECT,
		EXIT,
		TOGGLE_AWEIGHTING,
		TOGGLE_HANNWINDOW,
		HELP,
		TOGGLE_FULLSCREEN,
		MARK_BEAT
	};

	struct KbBinding {
		int sdlCode;
		SonoroAction action;
	};

	class InputContext
	{
	public:
		InputContext();
		~InputContext();

		void pollInputs();
		
		bool actionActivated(SonoroAction action);
		bool sdlKeyActivated(int sdlCode);

	private:
		void pollKeyboard();

		KeyboardMap m_keyboard;
		std::map<SonoroAction, int> m_keyboardBindings;
	};
}

#endif /* INPUT_H */
