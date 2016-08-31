#ifndef SUPERFORMULA_H
#define SUPERFORMULA_H

#define POINT_COUNT 500
#define MEAN_HIST_SIZE 10

#define SHAPE_COUNT 5

#include <scene.h>

namespace so
{
	class SuperformulaScene : public Scene
	{
	public:
		struct SuperformulaParams
		{
			SDL_Point m_points[POINT_COUNT];
			float m_lastEval[POINT_COUNT];
			float a, b, m, n1, n2, n3;
			float red, grn, blu;
			float rotDirection;
			float radiusMultiplier;
		};

		SuperformulaScene(Sonoro &app);
		~SuperformulaScene();

		int initialize() override;
		void update() override;
		void draw() override;

		std::string sceneName() override { return "Superformula Scene"; }
	private:
		void updateParams();
		float superformula(SuperformulaParams &params, float angle);

		float m_lastMeans[MEAN_HIST_SIZE];
		SuperformulaParams m_params[SHAPE_COUNT];
		SuperformulaParams m_lastParams[SHAPE_COUNT];
		SuperformulaParams m_nextParams[SHAPE_COUNT];
		float m_currentRotation;
		bool m_inTransition;
		Uint32 m_msCounter;
		float m_prevMean;
		float m_spikeLen;
	};
}

#endif /* SUPERFORMULA_H */