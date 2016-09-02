#include <raw_scene.h>

using namespace so;

RawScene::RawScene(Sonoro & app) :
	Scene(app)
{
}

RawScene::~RawScene()
{
}

int RawScene::initialize()
{
	return 0;
}

void RawScene::update()
{
	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();
	
	AudioContext::Values &samples = m_app.getAudioContext().getRawSamples();

	float multiplier = 400.0f;

	for (int i = 0; i < AC_IN_SIZE; i++)
	{
		float x = ((float)i / (AC_IN_SIZE - 1)) * w;

		float normalizedSample = (samples.values[i] - samples.min) / (samples.max - samples.min);

		float y = (h / 2) + (normalizedSample * multiplier) - (multiplier / 2);
		m_points[i].x = (int)x;
		m_points[i].y = (int)((y + m_points[i].y) / 2);
	}
}

void RawScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	SDL_RenderDrawLines(ren, m_points, AC_IN_SIZE);
}
