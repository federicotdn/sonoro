#include <line_scene.h>

using namespace so;

LineScene::LineScene(RenderContext & renderContext, AudioContext & audioContext, KeyboardMap & keyboard) :
	Scene(renderContext, audioContext, keyboard),
	m_points{0}
{
}

LineScene::~LineScene()
{
}

int LineScene::update()
{
	float w = (float)m_renderContext.getWindowWidth();
	float h = (float)m_renderContext.getWindowHeight();

	float *samples = m_audioContext.getSamples();
	float *frequencies = m_audioContext.getSampleFrequencies();

	for (int i = 0; i < AUDIO_CONTEXT_HALF_DATALEN; i++)
	{
		float x = ((float)i / (AUDIO_CONTEXT_HALF_DATALEN - 1)) * w;
		float y = (h / 2) - ((samples[i] * 500) - 250);
		m_points[i].x = (int)x;
		m_points[i].y = (int)y;
	}

	return 0;
}

void LineScene::draw()
{
	SDL_Renderer *ren = m_renderContext.getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	
	SDL_RenderDrawLines(ren, m_points, AUDIO_CONTEXT_HALF_DATALEN);
}

