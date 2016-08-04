#include <bars_scene.h>

using namespace so;

BarsScene::BarsScene(Sonoro &app) :
	Scene(app),
	m_rects{ 0 }
{
}

BarsScene::~BarsScene()
{
}

void BarsScene::update()
{
	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();

	float *samples = m_app.getAudioContext().getSamples();
	float *frequencies = m_app.getAudioContext().getSampleFrequencies();

	float barWidth = w / BAR_COUNT;
	for (int i = 0; i < BAR_COUNT; i++)
	{
		float val = 0;
		for (int j = 0; j < BAR_DATA_WIDTH; j++)
		{
			val += samples[(BAR_DATA_WIDTH * i) + j];
		}
		val /= BAR_DATA_WIDTH;
		
		m_rects[i].h = (int)(val * h);
		m_rects[i].w = (int)barWidth;
		m_rects[i].y = (int)(h - m_rects[i].h);
		m_rects[i].x = (int)(i * barWidth);
	}
}

void BarsScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	SDL_RenderDrawRects(ren, m_rects, BAR_COUNT);
}

