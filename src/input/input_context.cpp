#include <input_context.h>

#include <GLFW/glfw3.h>

#include <vector>
using namespace so;

InputContext::InputContext(RenderContext &renderContext) :
	m_renderContext(renderContext)
{
	// ====== Keyboard Bindings ======
	std::vector<KbBinding> kbindingsVec = {
		{ GLFW_KEY_F1, TOGGLE_HUD },
		{ GLFW_KEY_SPACE, NEXT_SCENE },
		{ GLFW_KEY_C, PREV_SCENE },
		{ GLFW_KEY_P, SMOOTH_UP },
		{ GLFW_KEY_O, SMOOTH_DOWN },
		{ GLFW_KEY_UP, UP },
		{ GLFW_KEY_DOWN, DOWN },
		{ GLFW_KEY_RIGHT, RIGHT },
		{ GLFW_KEY_LEFT, LEFT },
		{ GLFW_KEY_ENTER, SELECT },
		{ GLFW_KEY_ESCAPE, EXIT },
		{ GLFW_KEY_A, TOGGLE_AWEIGHTING },
		{ GLFW_KEY_W, TOGGLE_HANNWINDOW },
		{ GLFW_KEY_H, HELP },
		{ GLFW_KEY_F, TOGGLE_FULLSCREEN },
		{ GLFW_KEY_R, RELOAD_ASSETS }
	};
	// ==== End Keyboard Bindings ====

	for (int i = 0; i < kbindingsVec.size(); i++)
	{
		m_keyboardBindings[kbindingsVec[i].action] = kbindingsVec[i].glfwCode;
		m_lastKeyStates[kbindingsVec[i].glfwCode] = GLFW_RELEASE;
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

bool InputContext::actionHeld(SonoroAction action)
{
	return m_heldKeyboard[m_keyboardBindings[action]];
}

void InputContext::pollKeyboard()
{
	m_keyboard.clear();
	m_heldKeyboard.clear();

	for (auto &keyValue : m_keyboardBindings)
	{
		int keyboardCode = keyValue.second;
		int status = glfwGetKey(m_renderContext.getWindow(), keyboardCode);
		int lastStatus = m_lastKeyStates[keyboardCode];
		
		if (status == GLFW_RELEASE && lastStatus == GLFW_PRESS)
		{
			m_keyboard[keyboardCode] = true;
		}

		if (status == GLFW_PRESS)
		{
			m_heldKeyboard[keyboardCode] = true;
		}

		m_lastKeyStates[keyboardCode] = status;
	}
}