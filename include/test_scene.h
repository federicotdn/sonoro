#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <scene.h>

#include <program.h>

namespace so
{
	class TestScene : public Scene
	{
	public:
		TestScene(Sonoro &app);
		~TestScene();

		int initialize() override;
		void update() override;
		void draw() override;

		std::string sceneName() override { return "Test Scene"; }

	private:
		Program *m_program;
	};
}

#endif /* TEST_SCENE_H */