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

	AudioContext::Values freqs = m_app.getAudioContext().getFrequencyAmplitudes();

	for (int i = 0; i < AC_OUT_SIZE; i++)
	{
		float x = ((float)i / (AC_OUT_SIZE - 1)) * w;

		float amplitude = freqs.values[i];
		amplitude = (amplitude - freqs.min) / (freqs.max - freqs.min);

		float y = (h / 2) - ((amplitude * 500) - 250);
		m_points[i].x = (int)x;
		m_points[i].y = (int)((y + m_points[i].y) / 2);
	}
}

void LineScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	
	SDL_RenderDrawLines(ren, m_points, AC_OUT_SIZE);
}

