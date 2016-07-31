#include <bars_scene.h>

using namespace so;

BarsScene::BarsScene(RenderContext & renderContext, AudioContext & audioContext, KeyboardMap & keyboard) :
	Scene(renderContext, audioContext, keyboard),
	m_rects{ 0 }
{
}

BarsScene::~BarsScene()
{
}

int BarsScene::update()
{
	float w = (float)m_renderContext.getWindowWidth();
	float h = (float)m_renderContext.getWindowHeight();

	float *samples = m_audioContext.getSamples();
	float *frequencies = m_audioContext.getSampleFrequencies();

	float barWidth = w / BAR_COUNT;
	for (int i = 0; i < BAR_COUNT; i++)
	{
		float val = 0;
		for (int j = 0; j < BAR_DATA_WIDTH; j++)
		{
			val += samples[(BAR_DATA_WIDTH * i) + j];
		}
		val /= BAR_DATA_WIDTH;
		
		m_rects[i].h = val * 500;
		m_rects[i].w = barWidth;
		m_rects[i].y = h - m_rects[i].h;
		m_rects[i].x = i * barWidth;
	}

	return 0;
}

void BarsScene::draw()
{
	SDL_Renderer *ren = m_renderContext.getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	SDL_RenderDrawRects(ren, m_rects, BAR_COUNT);
}

