#ifndef LINE_SCENE_H
#define LINE_SCENE_H

#include <scene.h>
#include <shapes.h>

namespace so
{
	class LineScene : public Scene
	{
	public:
		LineScene(Sonoro &app);
		~LineScene();

		void update() override;
		void draw() override;

		std::string sceneName() override { return "Line Scene"; }
	
	private:
		Point m_points[AC_OUT_SIZE];
	};
}

#endif /* LINE_SCENE_H */