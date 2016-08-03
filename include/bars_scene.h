#ifndef BARS_SCENE_H
#define BARS_SCENE_H

#include <scene.h>

#define BAR_DATA_WIDTH 2
#define BAR_COUNT (AC_OUT_SIZE / BAR_DATA_WIDTH)

namespace so
{
	class BarsScene : public Scene
	{
	public:
		BarsScene(RenderContext &renderContext, AudioContext &audioContext, KeyboardMap &keyboard);
		~BarsScene();

		int update() override;
		void draw() override;

	private:
		SDL_Rect m_rects[BAR_COUNT];
	};
}

#endif /* BARS_SCENE_H */