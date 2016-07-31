#ifndef AUDIO_CONTEXT_H
#define AUDIO_CONTEXT_H

#include <string>
#include <vector>

#include <portaudio.h>
#include <fftw3.h>

#include <constants.h>

#define DEFAULT_CHAN_COUNT 1
#define DEFAULT_FRAMES_PER_BUFFER 512
#define DEFAULT_SMOOTHING 0.5f
#define DEFAULT_OUT_SIZE ((DEFAULT_FRAMES_PER_BUFFER / 2) + 1)
#define AUDIO_CONTEXT_DATALEN DEFAULT_OUT_SIZE
#define AUDIO_CONTEXT_HALF_DATALEN (AUDIO_CONTEXT_DATALEN / 2)

#define AC_USE_HANN_WINDOW 0

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
		float *getSampleFrequencies() { return m_sampleFrequencies; }
		
		std::string deviceName(int index);
		std::vector<int> inputDeviceList();
		int getDefaultInputDevice();

	private:
		float sampleComplexToReal(fftwf_complex c);

		bool m_initialized;
		std::string m_error;

		/* PortAudio */
		PaStream *m_instream;

		/* FFTW */
		fftwf_plan m_plan;
		float *m_inBuf;
		fftwf_complex *m_outBuf;
		float m_processedSamples[DEFAULT_OUT_SIZE];
		float m_sampleFrequencies[DEFAULT_OUT_SIZE];
	};
}

#endif /* AUDIO_CONTEXT_H */
