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
	float w = m_renderContext.getWindowWidth();
	float h = m_renderContext.getWindowHeight();

	float *samples = m_audioContext.getSamples();
	for (int i = 0; i < AUDIO_CONTEXT_DATALEN; i++)
	{
		float x = ((float)i / (AUDIO_CONTEXT_DATALEN - 1)) * w;
		float y = (h - 20) - (h - 20) * samples[i];
		m_points[i].x = (int)x;
		m_points[i].y = (int)y;
	}

	return 0;
}

void LineScene::draw()
{
	SDL_Renderer *ren = m_renderContext.getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	
	SDL_RenderDrawLines(ren, m_points, AUDIO_CONTEXT_DATALEN);
	
	//SDL_Rect r = {10,10,10,10};
	//SDL_RenderDrawRect(ren, &r);
}

