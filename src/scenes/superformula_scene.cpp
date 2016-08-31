#include <superformula_scene.h>
#include <iostream>
#include <math_utils.h>

#define ROTATION_STEP -0.001f
#define TRANSITION_DURATION_MS 2000
#define WAIT_DURATION_MS 1500
#define BEATS_PER_SHAPE 10

#define MEAN_DELTA_TOLERANCE 0.1f
#define RADIUS_MULTIPLIER 300.0f
#define MIN_VISIBLE_RADIUS 1.0f
#define MAX_VISIBLE_RADIUS 1000.0f

#define MIN_SPIKE_LEN 0
#define MAX_SPIKE_LEN 250
#define SPIKE_STEP 5.0f
#define SPIKE_JUMP 95.0f

using namespace so;

SuperformulaScene::SuperformulaScene(Sonoro & app) :
	Scene(app),
	m_currentRotation(0),
	m_msCounter(TRANSITION_DURATION_MS + 1),
	m_inTransition(false),
	m_prevMean(0),
	m_lastMeans{0},
	m_spikeLen(MIN_SPIKE_LEN)
{
	// m: rotational symmetry
	// n1, n2, n3: form
	// a, b: expansion coefficients

	for (int i = 0; i < SHAPE_COUNT; i++)
	{
		m_params[i].a = m_params[i].b = 1;
		m_params[i].m = 10;
		m_params[i].n1 = 5;
		m_params[i].n2 = 18;
		m_params[i].n3 = 18;
		m_params[i].red = m_params[i].grn = m_params[i].blu = 1;
		m_params[i].radiusMultiplier = MathUtils::rand() * 0.2f + 0.8f;
		m_params[i].rotDirection = i % 2 == 0 ? 1 : -1;
		m_lastParams[i] = m_params[i];
	}
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

	float xOffset = w / 2;
	float yOffset = h / 2;

	AudioContext::Samples &rawSamples = m_app.getAudioContext().getRawSamples();
	float mean = (rawSamples.mean - rawSamples.min) / (rawSamples.max - rawSamples.min);
	float histMean = 0;

	for (int i = 1; i < MEAN_HIST_SIZE; i++)
	{
		m_lastMeans[i - 1] = m_lastMeans[i];
		histMean += m_lastMeans[i];
	}

	m_lastMeans[MEAN_HIST_SIZE - 1] = mean;
	histMean /= MEAN_HIST_SIZE;

	bool beat = m_app.getAudioContext().onBeat();
	if (beat)
	{
		m_currentRotation += PI_FLOAT / 5;
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

	m_spikeLen += -SPIKE_STEP + (beat ? SPIKE_JUMP : 0);
	m_spikeLen = MathUtils::clamp<int>(MIN_SPIKE_LEN, MAX_SPIKE_LEN, (int)m_spikeLen);

	float radiusMean = 0;

	for (int j = 0; j < SHAPE_COUNT; j++)
	{
		SuperformulaParams &params = m_params[j];

		for (int i = 0; i < POINT_COUNT; i++)
		{
			float pctg = (float)i / (POINT_COUNT - 1);
			float angle = pctg * 2 * PI_FLOAT;
			float result = superformula(params, angle);
			float r = ((result + params.m_lastEval[i]) / 2) * histMean * RADIUS_MULTIPLIER;
			r += m_spikeLen * params.radiusMultiplier;
			params.m_lastEval[i] = result;

			radiusMean += r;
			float x = r * cosf(angle + m_currentRotation * params.rotDirection) + xOffset;
			float y = r * sinf(angle + m_currentRotation * params.rotDirection) + yOffset;

			params.m_points[i].x = (int)x;
			params.m_points[i].y = (int)y;
		}

		radiusMean /= POINT_COUNT;

		params.m_points[0].x = params.m_points[POINT_COUNT - 1].x;
		params.m_points[0].y = params.m_points[POINT_COUNT - 1].y;
	}

	m_prevMean = mean;
}

void SuperformulaScene::draw()
{
	SDL_Renderer *ren = m_app.getRenderContext().getRenderer();

	for (int i = 0; i < SHAPE_COUNT; i++)
	{
		SuperformulaParams &params = m_params[i];

		Uint8 r = (Uint8)(params.red * 255);
		Uint8 g = (Uint8)(params.grn * 255);
		Uint8 b = (Uint8)(params.blu * 255);

		SDL_SetRenderDrawColor(ren, r, g, b, 255);
		SDL_RenderDrawLines(ren, params.m_points, POINT_COUNT);
	}
}

void SuperformulaScene::updateParams()
{
	float pctg = (float)m_msCounter / TRANSITION_DURATION_MS;

	bool transitioning = false;
	bool genNextTransition = false;
	if (m_inTransition)
	{
		if (m_msCounter > TRANSITION_DURATION_MS)
		{
			m_inTransition = false;
			m_msCounter = 0;
		}
		else
		{
			transitioning = true;
		}
	}
	else if (m_msCounter > WAIT_DURATION_MS && m_app.getAudioContext().getBeatCount() % BEATS_PER_SHAPE == 0)
	{
		m_inTransition = true;
		m_msCounter = 0;
		genNextTransition = true;
	}

	for (int i = 0; i < SHAPE_COUNT; i++)
	{
		SuperformulaParams &params = m_params[i];

		if (transitioning)
		{
			params.a = MathUtils::lerp(m_lastParams[i].a, m_nextParams[i].a, pctg);
			params.b = MathUtils::lerp(m_lastParams[i].b, m_nextParams[i].b, pctg);
			params.m = MathUtils::lerp(m_lastParams[i].m, m_nextParams[i].m, pctg);
			params.n1 = MathUtils::lerp(m_lastParams[i].n1, m_nextParams[i].n1, pctg);
			params.n2 = MathUtils::lerp(m_lastParams[i].n2, m_nextParams[i].n2, pctg);
			params.n3 = MathUtils::lerp(m_lastParams[i].n3, m_nextParams[i].n3, pctg);
			params.red = MathUtils::lerp(m_lastParams[i].red, m_nextParams[i].red, pctg);
			params.grn = MathUtils::lerp(m_lastParams[i].grn, m_nextParams[i].grn, pctg);
			params.blu = MathUtils::lerp(m_lastParams[i].blu, m_nextParams[i].blu, pctg);
		}
		else if (genNextTransition)
		{
			m_lastParams[i] = m_params[i];
			m_nextParams[i].a = -10 + MathUtils::rand() * 30;
			m_nextParams[i].b = -10 + MathUtils::rand() * 30;
			m_nextParams[i].m = -10 + MathUtils::rand() * 2000;
			m_nextParams[i].n1 = -3 + MathUtils::rand() * 8;
			m_nextParams[i].n2 = -3 + MathUtils::rand() * 10;
			m_nextParams[i].n3 = -3 + MathUtils::rand() * 10;
			m_nextParams[i].red = MathUtils::rand();
			m_nextParams[i].grn = MathUtils::rand();
			m_nextParams[i].blu = MathUtils::rand();
		}
	}
}

float SuperformulaScene::superformula(SuperformulaParams &params, float angle)
{
	float a = params.a != 0 ? params.a : 1;
	float b = params.b != 0 ? params.b : 1;
	float n1 = params.n1 != 0 ? params.n1 : 1;

	float l = fabs(cosf(params.m * angle / 4.0f) / a);
	l = powf(l, params.n2);

	float r = fabs(sinf(params.m * angle / 4.0f) / b);
	l = powf(l, params.n3);

	return powf(r + l, -1 / n1);
}
