#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <map>

#include <render_context.h>
#include <audio_context.h>
#include <text_renderer.h>
#include <constants.h>
#include <input.h>

namespace so
{
	class Visualizer
	{
	public:
		Visualizer(RenderContext &renderContext, AudioContext &audioContext);
		~Visualizer();

		int initialize();
		void run();
	private:
		void pollKeyboard();

		RenderContext &m_renderContext;
		AudioContext &m_audioContext;

		TextRenderer m_info;
		KeyboardMap m_keyboard;

		bool m_initialized;
	};
}

#endif /* VISUALIZER_H */