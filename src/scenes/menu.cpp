#include <menu.h>
#include <math_utils.h>

using namespace so;

Menu::Menu(Sonoro &app) :
	Scene(app),
	m_currentSelection(0),
	m_points()
{
}

Menu::~Menu()
{
}

int Menu::initialize()
{
	updateCircle(true);
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
		//m_info.appendln(name);
	}
}

void Menu::reset()
{
	m_menuIndexToPAIndex.clear();
	showInputDevices();
}

void Menu::draw()
{
	//SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	//SDL_Texture *infoTex = m_info.getTexture(m_app.getRenderContext());
	//SDL_RenderCopy(ren, infoTex, NULL, m_info.getRect());

	//SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	//SDL_RenderDrawLines(ren, m_points, AC_OUT_SIZE);
}

void Menu::updateCircle(bool initial)
{
	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();

	AudioContext::Values &freqs = m_app.getAudioContext().getFrequencyAmplitudes();
	float *samples = freqs.values;
	float baseRadius = 90.0f;
	float diffRadius = 70.0f;

	float xOffset = w / 2;
	float yOffset = h / 2 + 100;

	for (int i = 0; i < AC_OUT_SIZE; i++)
	{
		float r = baseRadius + diffRadius * ((samples[i] - freqs.min) / (freqs.max - freqs.min));
		float pctg = (float)i / (AC_OUT_SIZE - 1);
		float x = r * cosf(2 * PI_FLOAT * pctg) + xOffset;
		float y = r * sinf(2 * PI_FLOAT * pctg) + yOffset;
		if (initial)
		{
			m_points[i].x = (int)x;
			m_points[i].y = (int)y;
		}
		else
		{
			m_points[i].x = (int)((x + m_points[i].x) / 2);
			m_points[i].y = (int)((y + m_points[i].y) / 2);
		}
	}
	
	// Tie the ends
	float xAvg = (float)(m_points[0].x + m_points[AC_OUT_SIZE - 1].x) / 2;
	m_points[0].x = (int)xAvg;
	m_points[AC_OUT_SIZE - 1].x = (int)xAvg;
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
				//m_info.appendln("Error setting device!");
			}
			else
			{
				//m_info.appendln("Device set correctly: PortAudio ID " + std::to_string(device));
			}
		}
	}
	else
	{
		if (m_app.getInputContext().actionActivated(SonoroAction::UP))
		{
			m_currentSelection++;
			m_currentSelection = MathUtils::clamp(0, (int)m_menuIndexToPAIndex.size(), m_currentSelection);
			reset();
		}
		else if (m_app.getInputContext().actionActivated(SonoroAction::DOWN))
		{
			m_currentSelection--;
			m_currentSelection = MathUtils::clamp(0, (int)m_menuIndexToPAIndex.size(), m_currentSelection);
			reset();
		}
	}

	updateCircle(false);
}


