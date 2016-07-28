#include <menu.h>

using namespace so;

Menu::Menu(RenderContext &renderContext, AudioContext &audioContext, KeyboardMap &keyboard) :
	Scene(renderContext, audioContext, keyboard)
{
}

Menu::~Menu()
{
}

int Menu::initialize()
{
	int w;
	m_renderContext.getWindowSize(&w, nullptr);

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
	m_info.clear();
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

void Menu::reset()
{
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
	if (m_keyboard[SDLK_e])
	{
		return 1;
	}

	return 0;
}


