#ifndef SUPERFORMULA_H
#define SUPERFORMULA_H

#define POINT_COUNT 500
#define MEAN_HIST_SIZE 5

#define SHAPE_COUNT 5

#include <scene.h>
#include <camera.h>
#include <model.h>
#include <framebuffer.h>
#include <glm/vec3.hpp>

namespace so
{
	class SuperformulaScene : public Scene
	{
	public:
		struct SuperformulaParams
		{
			glm::vec3 m_points[POINT_COUNT];
			float m_lastEval[POINT_COUNT];
			float a, b, m, n1, n2, n3;
			float red, grn, blu;
			int rotDirection;
			float radiusMultiplier;
		};

		SuperformulaScene(Sonoro &app);
		~SuperformulaScene();

		int initialize() override;
		void activate() override;
		void update() override;
		void draw() override;

		std::string sceneName() override { return "Superformula Scene"; }
	private:
		void updateParams();
		float superformula(SuperformulaParams &params, float angle);
		void reloadShaders();

		Camera m_cam;
		std::vector<Model*> m_models;
		Program *m_program;

		Model *m_backgroundModel;
		Program *m_backgroundProgram;

		Model *m_backgroundFboModel;
		Program *m_framebufferProgram;
		Framebuffer m_framebuffer;

		float m_lastMeans[MEAN_HIST_SIZE];
		SuperformulaParams m_params[SHAPE_COUNT];
		SuperformulaParams m_lastParams[SHAPE_COUNT];
		SuperformulaParams m_nextParams[SHAPE_COUNT];
		float m_currentRotation;
		bool m_inTransition;
		uint32_t m_msCounter;
		float m_spikeLen;
		bool m_drawBackground;
	};
}

#endif /* SUPERFORMULA_H */