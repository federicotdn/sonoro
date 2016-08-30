#ifndef BEAT_SCENE_H
#define BEAT_SCENE_H

#include <scene.h>

namespace so
{
	class BeatScene : public Scene
	{
	public:
		BeatScene(Sonoro &app);
		~BeatScene();

		void update() override;
		void draw() override;

		std::string sceneName() override { return "Beat Scene"; }

	private:
		float m_size;
		float m_lastMean;
		float m_meanTolerance;
	};
}

#endif /* BEAT_SCENE_H */