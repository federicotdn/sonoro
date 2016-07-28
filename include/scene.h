#ifndef SCENE_H
#define SCENE_H

#include <render_context.h>
#include <audio_context.h>
#include <input.h>

namespace so
{
	class Scene
	{
	public:
		Scene(RenderContext &renderContext, AudioContext &audioContext, KeyboardMap &keyboard) :
			m_renderContext(renderContext),
			m_audioContext(audioContext),
			m_keyboard(keyboard)
		{ }

		virtual ~Scene()
		{ }

		virtual int initialize() { return 0; }
		virtual int update() = 0;
		virtual void draw() = 0;

	protected:
		RenderContext &m_renderContext;
		AudioContext &m_audioContext;
		KeyboardMap &m_keyboard;
	};
}

#endif /* SCENE_H */