#include <bars_scene.h>
#include <math_utils.h>

#define SMOOTHING_STEP 0.05f

using namespace so;

BarsScene::BarsScene(Sonoro &app) :
	Scene(app),
	m_rects{ 0 },
	m_barDataWidth(DEFAULT_BAR_DATA_WIDTH),
	m_smoothing(0.5f)
{
}

BarsScene::~BarsScene()
{
}

void BarsScene::update()
{
	if (m_app.getInputContext().actionActivated(SonoroAction::UP))
	{
		m_barDataWidth++;
	}
	else if (m_app.getInputContext().actionActivated(SonoroAction::DOWN))
	{
		m_barDataWidth--;
	}

	if (m_app.getInputContext().actionActivated(SonoroAction::RIGHT))
	{
		m_smoothing += SMOOTHING_STEP;
	}
	else if (m_app.getInputContext().actionActivated(SonoroAction::LEFT))
	{
		m_smoothing -= SMOOTHING_STEP;
	}

	m_barDataWidth = MathUtils::clamp(1, 10, m_barDataWidth);
	m_smoothing = MathUtils::clamp(0.0f, 1.0f, m_smoothing);

	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();

	float *samples = m_app.getAudioContext().getProcessedSamples();

	int barCount = AC_OUT_SIZE / m_barDataWidth;

	float barWidth = w / barCount;
	for (int i = 0; i < barCount; i++)
	{
		float val = 0;
		for (int j = 0; j < m_barDataWidth; j++)
		{
			val += samples[(m_barDataWidth * i) + j];
		}
		val /= m_barDataWidth;
		
		m_rects[i].h = (int)((val * h) * (1 - m_smoothing) + m_rects[i].h * m_smoothing);

		m_rects[i].w = (int)barWidth;
		m_rects[i].y = (int)(h - m_rects[i].h);
		m_rects[i].x = (int)(i * barWidth);
	}
}

void BarsScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();

	uint8_t gb = m_app.getAudioContext().onBeat() ? 0 : 255;

	SDL_SetRenderDrawColor(ren, 255, gb, gb, 255);

	int barCount = AC_OUT_SIZE / m_barDataWidth;
	SDL_RenderDrawRects(ren, m_rects, barCount);
}

