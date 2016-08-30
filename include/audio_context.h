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
#define DEFAULT_SMOOTHING 0.95f
#define DEFAULT_BACK_BUFFER_COUNT 35

#define AC_RAW_OUT_SIZE ((DEFAULT_FRAMES_PER_BUFFER / 2) + 1)
// Skip first value of m_outBuf
#define AC_RAW_OUT_OFFSET 1

// Public:
#define AC_OUT_SIZE (AC_RAW_OUT_SIZE / 2) // (only use values in 0-10k Hz range)
#define AC_IN_SIZE DEFAULT_FRAMES_PER_BUFFER

namespace so
{
	class AudioContext
	{
	public:
		struct Samples
		{
			Samples() :
				samplesSize(0),
				samples(nullptr),
				min(0), max(0), mean(0)
			{ }

			int samplesSize;
			float *samples;
			float min, max;
			float mean;
		};

		AudioContext();
		~AudioContext();

		int initialize();
		std::string getError() { return m_error; }
		
		int setInputDevice(int index);
		int startStream();
		int stopStream();
		void processSamples();
		float *getProcessedSamples() { return m_processedSamples; }
		Samples &getRawSamples() { return m_rawSamples; }
		float *getSampleFrequencies() { return m_sampleFrequencies; }
		
		void addSmoothing(float val);
		float getSmoothing() { return m_smoothing; }
		void setHannWindowEnabled(bool enabled);
		bool getHannWindowEnabled() { return m_hannWindowEnabled; }
		void setAWeightingEnabled(bool enabled);
		bool getAWeightingEnabled() { return m_aWeightingEnabled; }

		void updateBeat(uint32_t msElapsed, bool beatMarked);
		bool onBeat();
		int getBPM();
		bool isSettingBPM();

		std::string deviceName(int index);
		std::vector<int> inputDeviceList();
		int getDefaultInputDevice();

	private:
		float sampleToDb(fftwf_complex c);
		void initializeSampleStructs();

		bool m_initialized;
		std::string m_error;

		uint32_t m_msLastBeat;
		uint32_t m_msBetweenBeats;
		uint32_t m_msBeatSetTimeoutCounter;
		uint32_t m_msBeatSetPeriod;
		bool m_onBeat;
		int m_beatCount;

		/* PortAudio */
		PaStream *m_instream;
		float m_smoothing;
		bool m_hannWindowEnabled;
		bool m_aWeightingEnabled;

		/* FFTW */
		fftwf_plan m_plan;
		float *m_inBuf;
		fftwf_complex *m_outBuf;
		float m_processedSamples[AC_OUT_SIZE];
		float m_sampleFrequencies[AC_RAW_OUT_SIZE];

		/* Packaged Samples data */
		Samples m_rawSamples;

		int m_lastBackBuffer;
		fftwf_complex m_backBuffers[DEFAULT_BACK_BUFFER_COUNT][AC_OUT_SIZE];
	};
}

#endif /* AUDIO_CONTEXT_H */
