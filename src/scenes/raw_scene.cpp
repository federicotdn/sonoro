#include <raw_scene.h>

using namespace so;

RawScene::RawScene(Sonoro & app) :
	Scene(app),
	m_lastMean(0),
	m_meanTolerance(0.1f)
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
	if (m_app.getInputContext().actionActivated(SonoroAction::UP))
	{
		m_meanTolerance += 0.01f;
	}
	else if (m_app.getInputContext().actionActivated(SonoroAction::DOWN))
	{
		m_meanTolerance -= 0.01f;
	}

	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();

	float *samples = m_app.getAudioContext().getRawSamples().samples;
	float maxSample = std::numeric_limits<float>::lowest();
	float minSample = std::numeric_limits<float>::max();
	float mean = 0;

	for (int i = 0; i < AC_IN_SIZE; i++)
	{
		if (samples[i] > maxSample)
		{
			maxSample = samples[i];
		}

		if (samples[i] < minSample)
		{
			minSample = samples[i];
		}

		mean += samples[i];
	}

	mean /= AC_IN_SIZE;
	mean = (mean - minSample) / (maxSample - minSample);

	float normalizedSamples[AC_IN_SIZE];
	for (int i = 0; i < AC_IN_SIZE; i++)
	{
		normalizedSamples[i] = (samples[i] - minSample) / (maxSample - minSample);
	}

	float multiplier = 1000.0f;
	if (fabs(mean - m_lastMean) > m_meanTolerance)
	{
		multiplier = 3000.0f;
	}

	for (int i = 0; i < AC_IN_SIZE; i++)
	{
		float x = ((float)i / (AC_IN_SIZE - 1)) * w;
		float y = (h / 2) - (samples[i] * multiplier);
		m_points[i].x = (int)x;
		m_points[i].y = (int)((y + m_points[i].y) / 2);
	}

	m_lastMean = mean;
}

void RawScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	SDL_RenderDrawLines(ren, m_points, AC_IN_SIZE);
}
