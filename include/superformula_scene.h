#ifndef SUPERFORMULA_H
#define SUPERFORMULA_H

#define POINT_COUNT 500
#define MEAN_HIST_SIZE 50

#include <scene.h>

namespace so
{
	class SuperformulaScene : public Scene
	{
	public:
		struct SuperformulaParams
		{
			float a, b, m, n1, n2, n3;
			float red, grn, blu;
		};

		SuperformulaScene(Sonoro &app);
		~SuperformulaScene();

		int initialize() override;
		void update() override;
		void draw() override;

		std::string sceneName() override { return "Superformula Scene"; }
	private:
		void updateParams();
		float superformula(float angle);

		SDL_Point m_points[POINT_COUNT];
		float m_lastEval[POINT_COUNT];
		float m_lastMeans[MEAN_HIST_SIZE];
		SuperformulaParams m_params;
		SuperformulaParams m_lastParams;
		SuperformulaParams m_nextParams;
		float m_currentRotation;
		bool m_inTransition;
		Uint32 m_msCounter;
		float m_prevMean;
		float m_meanTolerance;
	};
}

#endif /* SUPERFORMULA_H */