#include <input_context.h>

#include <vector>
using namespace so;

InputContext::InputContext()
{
	// ====== Keyboard Bindings ======
	std::vector<KbBinding> kbindingsVec = {
		{ SDLK_F1, TOGGLE_HUD },
		{ SDLK_SPACE, NEXT_SCENE },
		{ SDLK_c, PREV_SCENE },
		{ SDLK_p, SMOOTH_UP },
		{ SDLK_o, SMOOTH_DOWN },
		{ SDLK_UP, UP },
		{ SDLK_DOWN, DOWN },
		{ SDLK_RETURN, SELECT },
		{ SDLK_ESCAPE, EXIT },
		{ SDLK_a, TOGGLE_AWEIGHTING },
		{ SDLK_w, TOGGLE_HANNWINDOW },
		{ SDLK_h, HELP }
	};
	// ==== End Keyboard Bindings ====

	for (int i = 0; i < kbindingsVec.size(); i++)
	{
		m_keyboardBindings[kbindingsVec[i].action] = kbindingsVec[i].sdlCode;
	}
}

InputContext::~InputContext()
{
}

void InputContext::pollInputs()
{
	pollKeyboard();
}

bool InputContext::actionActivated(SonoroAction action)
{
	return m_keyboard[m_keyboardBindings[action]];
}

void InputContext::pollKeyboard()
{
	m_keyboard.clear();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			m_keyboard[event.key.keysym.sym] = false;
			break;
		case SDL_KEYUP:
			m_keyboard[event.key.keysym.sym] = true;
			break;
		}
	}
}