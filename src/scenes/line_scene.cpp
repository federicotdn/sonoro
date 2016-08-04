#include <line_scene.h>

using namespace so;

LineScene::LineScene(Sonoro &app) :
	Scene(app),
	m_points{0}
{
}

LineScene::~LineScene()
{
}

void LineScene::update()
{
	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();

	float *samples = m_app.getAudioContext().getSamples();
	float *frequencies = m_app.getAudioContext().getSampleFrequencies();

	for (int i = 0; i < AC_OUT_SIZE; i++)
	{
		float x = ((float)i / (AC_OUT_SIZE - 1)) * w;
		float y = (h / 2) - ((samples[i] * 500) - 250);
		m_points[i].x = (int)x;
		m_points[i].y = (int)y;
	}
}

void LineScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	
	SDL_RenderDrawLines(ren, m_points, AC_OUT_SIZE);
}

