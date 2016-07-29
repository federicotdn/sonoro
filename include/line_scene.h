#ifndef LINE_SCENE_H
#define LINE_SCENE_H

#include <scene.h>

namespace so
{
	class LineScene : public Scene
	{
	public:
		LineScene(RenderContext &renderContext, AudioContext &audioContext, KeyboardMap &keyboard);
		~LineScene();

		int update() override;
		void draw() override;
	
	private:
		SDL_Point m_points[AUDIO_CONTEXT_DATALEN];
	};
}

#endif /* LINE_SCENE_H */