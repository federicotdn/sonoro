#ifndef INPUT_H
#define INPUT_H

#include <render_context.h>
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
		RELOAD_ASSETS
	};

	struct KbBinding {
		int glfwCode;
		SonoroAction action;
	};

	class InputContext
	{
	public:
		InputContext(RenderContext &renderContext);
		~InputContext();

		void pollInputs();
		
		bool actionActivated(SonoroAction action);
		bool actionHeld(SonoroAction action);

	private:
		void pollKeyboard();

		RenderContext &m_renderContext;
		KeyboardMap m_keyboard;
		KeyboardMap m_heldKeyboard;
		std::map<SonoroAction, int> m_keyboardBindings;
		std::map<int, int> m_lastKeyStates;
	};
}

#endif /* INPUT_H */
