#include <menu.h>

using namespace so;

Menu::Menu(Sonoro &app) :
	Scene(app),
	m_currentSelection(0)
{
}

Menu::~Menu()
{
}

int Menu::initialize()
{
	int w = m_app.getRenderContext().getWindowWidth();

	int err = m_info.initialize(DEFAULT_FONT, 22, w);
	if (err)
	{
		return err;
	}

	m_info.setBackgroundColor({ 0, 0, 128 });

	reset();
	return 0;
}

void Menu::showInputDevices()
{
	int defaultInput = m_app.getAudioContext().getDefaultInputDevice();

	std::vector<int> devices = m_app.getAudioContext().inputDeviceList();
	for (int i = 0; i < devices.size(); i++)
	{
		std::string name = std::to_string(i + 1) + ": " + m_app.getAudioContext().deviceName(devices[i]);
		if (devices[i] == defaultInput)
		{
			name += " [DEFAULT]";
		}

		m_menuIndexToPAIndex[i + 1] = devices[i];
		m_info.appendln(name);
	}
}

void Menu::reset()
{
	m_menuIndexToPAIndex.clear();
	m_info.clear();
	m_info.appendln("Choose an input device: [ENTER to select, Up/Down to move]");
	m_info.appendln("(Entering 0 will choose the default device.)");
	m_info.appendln("Current choice: " + std::to_string(m_currentSelection));
	showInputDevices();
}

void Menu::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	SDL_Texture *infoTex = m_info.getTexture(m_app.getRenderContext());
	SDL_RenderCopy(ren, infoTex, NULL, m_info.getRect());
}

void Menu::update()
{
	if (m_app.getInputContext().actionActivated(SonoroAction::SELECT))
	{
		if (m_menuIndexToPAIndex.count(m_currentSelection) == 1 || m_currentSelection == 0)
		{
			int device = m_menuIndexToPAIndex[m_currentSelection];

			if (m_currentSelection == 0)
			{
				device = m_app.getAudioContext().getDefaultInputDevice();
			}

			int ret = m_app.getAudioContext().setInputDevice(device);
			if (ret)
			{
				m_info.appendln("Error setting device!");
			}
			else
			{
				m_info.appendln("Device set correctly: PortAudio ID " + std::to_string(device));
			}
		}
	}
	else
	{
		if (m_app.getInputContext().actionActivated(SonoroAction::UP))
		{
			m_currentSelection++;
			reset();
		}
		else if (m_app.getInputContext().actionActivated(SonoroAction::DOWN))
		{
			m_currentSelection--;
			reset();
		}
	}
}


