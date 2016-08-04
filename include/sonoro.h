#ifndef SONORO_H
#define SONORO_H

#include <map>

#include <render_context.h>
#include <audio_context.h>
#include <input_context.h>
#include <text_renderer.h>
#include <constants.h>

namespace so
{
	class Sonoro
	{
	public:
		Sonoro(RenderContext &renderContext, AudioContext &audioContext, InputContext &inputContext);
		~Sonoro();

		RenderContext &getRenderContext() { return m_renderContext; }
		AudioContext &getAudioContext() { return m_audioContext; }
		InputContext &getInputContext() { return m_inputContext; }

		int initialize();
		int run();
	private:
		void checkGlobalActions();

		RenderContext &m_renderContext;
		AudioContext &m_audioContext;
		InputContext &m_inputContext;

		TextRenderer m_info;
		bool m_initialized;
	};
}

#endif /* SONORO_H */