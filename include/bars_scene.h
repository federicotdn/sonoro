#ifndef BARS_SCENE_H
#define BARS_SCENE_H

#include <scene.h>
#include <shapes.h>

#define DEFAULT_BAR_DATA_WIDTH 2

namespace so
{
	class BarsScene : public Scene
	{
	public:
		BarsScene(Sonoro &app);
		~BarsScene();

		void update() override;
		void draw() override;

		std::string sceneName() override { return "Bars Scene"; }

	private:
		int m_barDataWidth;
		float m_smoothing;
		Rect m_rects[AC_OUT_SIZE];
	};
}

#endif /* BARS_SCENE_H */