#include <superformula_scene.h>
#include <iostream>
#include <math_utils.h>

#define ROTATION_STEP -0.001f;
#define TRANSITION_DURATION_MS 2500
#define WAIT_DURATION_MS 500

#define MEAN_DELTA_TOLERANCE 0.09f
#define RADIUS_MULTIPLIER 600.0f

using namespace so;

SuperformulaScene::SuperformulaScene(Sonoro & app) :
	Scene(app),
	m_currentRotation(0),
	m_msCounter(TRANSITION_DURATION_MS + 1),
	m_inTransition(false),
	m_prevMean(0),
	m_prevDeviation(0),
	m_lastEval{0},
	m_meanTolerance(MEAN_DELTA_TOLERANCE)
{
	// m: rotational symmetry
	// n1, n2, n3: form
	// a, b: expansion coefficients
	m_params.a = m_params.b = 1;
	m_params.m = 10;
	m_params.n1 = 5;
	m_params.n2 = 18;
	m_params.n3 = 18;
	m_params.red = m_params.grn = m_params.blu = 1;
	m_lastParams = m_params;
}

SuperformulaScene::~SuperformulaScene()
{
}

int SuperformulaScene::initialize()
{
	return 0;
}

void SuperformulaScene::update()
{
	m_msCounter += m_app.getRenderContext().getDeltaMs();

	updateParams();

	float w = (float)m_app.getRenderContext().getWindowWidth();
	float h = (float)m_app.getRenderContext().getWindowHeight();
	float *samples = m_app.getAudioContext().getProcessedSamples();

	float xOffset = w / 2;
	float yOffset = h / 2;

	float max = std::numeric_limits<float>::lowest();
	float min = std::numeric_limits<float>::max();
	float mean = 0;
	float deviation = 0;

	for (int i = 0; i < AC_OUT_SIZE; i++)
	{
		if (samples[i] > max)
		{
			max = samples[i];
		}

		if (samples[i] < min)
		{
			min = samples[i];
		}

		mean += samples[i];
	}

	mean /= AC_OUT_SIZE;

	if (fabs(mean - m_prevMean) < m_meanTolerance)
	{
		mean = m_prevMean;
	}
	else
	{
		m_currentRotation += mean / 10.0f;
	}

	m_currentRotation += ROTATION_STEP;
	while (m_currentRotation > 2 * PI_FLOAT)
	{
		m_currentRotation -= 2 * PI_FLOAT;
	}

	while (m_currentRotation < 0)
	{
		m_currentRotation += 2 * PI_FLOAT;
	}

	for (int i = 0; i < AC_OUT_SIZE; i++)
	{
		deviation += (samples[i] - mean) * (samples[i] - mean);
	}

	deviation = sqrtf(deviation / (AC_OUT_SIZE - 1));
	m_prevDeviation = deviation;

	for (int i = 0; i < POINT_COUNT; i++)
	{
		float pctg = (float)i / (POINT_COUNT - 1);
		float angle = pctg * 2 * PI_FLOAT;
		float result = superformula(angle);
		float r = ((result + m_lastEval[i]) / 2) * ((mean + m_prevMean) / 2) * RADIUS_MULTIPLIER;
		m_lastEval[i] = result;

		float x = r * cosf(angle + m_currentRotation) + xOffset;
		float y = r * sinf(angle + m_currentRotation) + yOffset;

		m_points[i].x = (int)x;
		m_points[i].y = (int)y;
	}

	m_points[0].x = m_points[POINT_COUNT - 1].x;
	m_points[0].y = m_points[POINT_COUNT - 1].y;

	m_prevMean = mean;
}

void SuperformulaScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();

	Uint8 r = (Uint8)(m_params.red * 255);
	Uint8 g = (Uint8)(m_params.grn * 255);
	Uint8 b = (Uint8)(m_params.blu * 255);

	SDL_SetRenderDrawColor(ren, r, g, b, 255);
	SDL_RenderDrawLines(ren, m_points, POINT_COUNT);
}

void SuperformulaScene::updateParams()
{
	if (m_inTransition)
	{
		if (m_msCounter > TRANSITION_DURATION_MS)
		{
			m_inTransition = false;
			m_msCounter = 0;
		}
		else
		{
			float pctg = (float)m_msCounter / TRANSITION_DURATION_MS;
			m_params.a = MathUtils::lerp(m_lastParams.a, m_nextParams.a, pctg);
			m_params.b = MathUtils::lerp(m_lastParams.b, m_nextParams.b, pctg);
			m_params.m = MathUtils::lerp(m_lastParams.m, m_nextParams.m, pctg);
			m_params.n1 = MathUtils::lerp(m_lastParams.n1, m_nextParams.n1, pctg);
			m_params.n2 = MathUtils::lerp(m_lastParams.n2, m_nextParams.n2, pctg);
			m_params.n3 = MathUtils::lerp(m_lastParams.n3, m_nextParams.n3, pctg);
			m_params.red = MathUtils::lerp(m_lastParams.red, m_nextParams.red, pctg);
			m_params.grn = MathUtils::lerp(m_lastParams.grn, m_nextParams.grn, pctg);
			m_params.blu = MathUtils::lerp(m_lastParams.blu, m_nextParams.blu, pctg);
		}
	}
	else if (m_msCounter > WAIT_DURATION_MS)
	{
		m_inTransition = true;
		m_msCounter = 0;

		m_lastParams = m_params;
		m_nextParams.a = -10 + MathUtils::rand() * 30;
		m_nextParams.b = -10 + MathUtils::rand() * 30;
		m_nextParams.m = -10 + MathUtils::rand() * 2000;
		m_nextParams.n1 = -3 + MathUtils::rand() * 10;
		m_nextParams.n2 = -3 + MathUtils::rand() * 11;
		m_nextParams.n3 = -3 + MathUtils::rand() * 11;
		m_nextParams.red = MathUtils::rand();
		m_nextParams.grn = MathUtils::rand();
		m_nextParams.blu = MathUtils::rand();
	}

	if (m_app.getInputContext().actionActivated(SonoroAction::UP))
	{
		m_meanTolerance += 0.01f;
	}
	else if (m_app.getInputContext().actionActivated(SonoroAction::DOWN))
	{
		m_meanTolerance -= 0.01f;
	}
}

float SuperformulaScene::superformula(float angle)
{
	float a = m_params.a != 0 ? m_params.a : 1;
	float b = m_params.b != 0 ? m_params.b : 1;
	float n1 = m_params.n1 != 0 ? m_params.n1 : 1;

	float l = fabs(cosf(m_params.m * angle / 4.0f) / a);
	l = powf(l, m_params.n2);

	float r = fabs(sinf(m_params.m * angle / 4.0f) / b);
	l = powf(l, m_params.n3);

	return powf(r + l, -1 / n1);
}
