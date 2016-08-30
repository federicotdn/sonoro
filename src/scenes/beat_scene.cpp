#include <beat_scene.h>

#define MIN_SIZE 100
#define MAX_SIZE 600
#define SIZE_STEP 10.0f

using namespace so;

BeatScene::BeatScene(Sonoro & app) :
	Scene(app),
	m_size(MIN_SIZE),
	m_lastMean(0),
	m_meanTolerance(0.1f)
{
}

BeatScene::~BeatScene()
{
}

void BeatScene::update()
{
	m_size -= SIZE_STEP;

	if (m_app.getInputContext().actionActivated(SonoroAction::UP))
	{
		m_meanTolerance += 0.01f;
	}
	else if (m_app.getInputContext().actionActivated(SonoroAction::DOWN))
	{
		m_meanTolerance -= 0.01f;
	}

	AudioContext::Samples &rawSamples = m_app.getAudioContext().getRawSamples();
	float mean = (rawSamples.mean - rawSamples.min) / (rawSamples.max - rawSamples.min);
	bool spike = (fabs(mean - m_lastMean) > m_meanTolerance);

	if (m_app.getInputContext().actionActivated(SonoroAction::UP) || spike)
	{
		m_size += 200;
	}

	if (m_size < MIN_SIZE)
	{
		m_size = MIN_SIZE;
	}
	else if (m_size > MAX_SIZE)
	{
		m_size = MAX_SIZE;
	}

	m_lastMean = mean;
}

void BeatScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();

	SDL_Rect rect;

	rect.w = rect.h = m_size;
	rect.x = (int)((w / 2) - (m_size / 2));
	rect.y = (int)((h / 2) - (m_size / 2));

	SDL_RenderDrawRect(ren, &rect);
}
