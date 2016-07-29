#ifndef AUDIO_CONTEXT_H
#define AUDIO_CONTEXT_H

#include <string>
#include <vector>

#include <portaudio.h>
#include <fftw3.h>

#define DEFAULT_CHAN_COUNT 1
#define DEFAULT_FRAMES_PER_BUFFER 64
#define DEFAULT_OUT_SIZE ((DEFAULT_FRAMES_PER_BUFFER / 2) + 1)
#define AUDIO_CONTEXT_DATALEN DEFAULT_OUT_SIZE

namespace so
{
	class AudioContext
	{
	public:
		AudioContext();
		~AudioContext();

		int initialize();
		std::string getError() { return m_error; }
		
		int setInputDevice(int index);
		int startStream();
		int stopStream();
		void processSamples();
		float *getSamples() { return m_processedSamples; }
		
		std::string deviceName(int index);
		std::vector<int> inputDeviceList();
		int getDefaultInputDevice();

	private:
		bool m_initialized;
		std::string m_error;

		/* PortAudio */
		PaStream *m_instream;

		/* FFTW */
		fftwf_plan m_plan;
		float *m_inBuf;
		fftwf_complex *m_outBuf;
		float m_processedSamples[DEFAULT_OUT_SIZE];
	};
}

#endif /* AUDIO_CONTEXT_H */
