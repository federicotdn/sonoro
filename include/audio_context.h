#ifndef AUDIO_CONTEXT_H
#define AUDIO_CONTEXT_H

#include <string>
#include <vector>

#include <portaudio.h>
#include <fftw3.h>

#include <constants.h>

#define DEFAULT_CHAN_COUNT 1
#define DEFAULT_FRAMES_PER_BUFFER 512
#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_SMOOTHING 0.05f
#define DEFAULT_BACK_BUFFER_COUNT 25

#define AC_RAW_OUT_SIZE ((DEFAULT_FRAMES_PER_BUFFER / 2) + 1)

// Use only values in 0-10k Hz range
#define AC_OUT_SIZE (AC_RAW_OUT_SIZE / 2)

#define AC_USE_HANN_WINDOW 0
#define AC_USE_A_WEIGHTING 1

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
		void addSmoothing(float val);
		float getSmoothing() { return m_smoothing; }
		
		std::string deviceName(int index);
		std::vector<int> inputDeviceList();
		int getDefaultInputDevice();

	private:
		float sampleToDb(fftwf_complex c);

		bool m_initialized;
		std::string m_error;

		/* PortAudio */
		PaStream *m_instream;

		/* FFTW */
		fftwf_plan m_plan;
		float *m_inBuf;
		fftwf_complex *m_outBuf;
		float m_processedSamples[AC_OUT_SIZE];
		float m_sampleFrequencies[AC_RAW_OUT_SIZE];

		float m_smoothing;
		int m_lastBackBuffer;
		fftwf_complex m_backBuffers[DEFAULT_BACK_BUFFER_COUNT][AC_OUT_SIZE];
	};
}

#endif /* AUDIO_CONTEXT_H */
