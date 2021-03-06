#ifndef RAW_SCENE_H
#define RAW_SCENE_H

#include <scene.h>
#include <shapes.h>

namespace so
{
	class RawScene : public Scene
	{
	public:
		RawScene(Sonoro &app);
		~RawScene();

		int initialize() override;
		void update() override;
		void draw() override;

		std::string sceneName() override { return "Raw Samples Scene"; }

	private:
		Point m_points[AC_IN_SIZE];
	};
}

#endif /* RAW_SCENE_H */