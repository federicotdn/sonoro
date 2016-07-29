#include <menu.h>

using namespace so;

Menu::Menu(RenderContext &renderContext, AudioContext &audioContext, KeyboardMap &keyboard, MenuResult &result) :
	Scene(renderContext, audioContext, keyboard),
	m_menuResult(result)
{
}

Menu::~Menu()
{
}

int Menu::initialize()
{
	int w = m_renderContext.getWindowWidth();

	int err = m_info.initialize(DEFAULT_FONT, 22, w);
	if (err)
	{
		return err;
	}

	showInputDevices();
	return 0;
}

void Menu::showInputDevices()
{
	int defaultInput = m_audioContext.getDefaultInputDevice();

	std::vector<int> devices = m_audioContext.inputDeviceList();
	for (int i = 0; i < devices.size(); i++)
	{
		std::string name = std::to_string(i + 1) + ": " + m_audioContext.deviceName(devices[i]);
		if (devices[i] == defaultInput)
		{
			name += " [DEFAULT]";
		}

		m_info.appendln(name);
	}
}

int Menu::getKeyboardNum()
{
	int keyboardNums[10] = {
		SDLK_0,
		SDLK_1,
		SDLK_2,
		SDLK_3,
		SDLK_4,
		SDLK_5,
		SDLK_6,
		SDLK_7,
		SDLK_8,
		SDLK_9,
	};
	
	for (int i = 0; i < sizeof(keyboardNums); i++) {
		if (m_keyboard[keyboardNums[i]])
		{
			return i;
		}
	}

	return -1;
}

void Menu::reset()
{
	m_info.clear();
	m_info.appendln("Choose an input device: (0 for default)");
	showInputDevices();
}

void Menu::draw()
{
	SDL_Renderer *ren = m_renderContext.getRenderer();
	SDL_Texture *infoTex = m_info.getTexture(m_renderContext);
	SDL_RenderCopy(ren, infoTex, NULL, m_info.getRect());
}

int Menu::update()
{
	int keyboardNum = getKeyboardNum();
	if (keyboardNum != -1)
	{
		std::vector<int> devices = m_audioContext.inputDeviceList();

		if (keyboardNum == 0)
		{
			m_menuResult.inputDeviceIndex = m_audioContext.getDefaultInputDevice();
			return -1;
		}
		else
		{
			int idx = keyboardNum - 1;
			if (idx < devices.size())
			{
				m_menuResult.inputDeviceIndex = devices[idx];
				return -1;
			}
		}
	}

	return 0;
}


