#ifndef AUDIO_CONTEXT_H
#define AUDIO_CONTEXT_H

#include <string>
#include <vector>

#include <portaudio.h>
#include <fftw3.h>

#include <constants.h>
#include <circular_array.h>

#define DEFAULT_CHAN_COUNT 1
#define DEFAULT_FRAMES_PER_BUFFER 512
#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_SMOOTHING 0.95f
#define DEFAULT_BACK_BUFFER_COUNT 35

#define ENERGY_HIST_SIZE 90 // Used by BPM Processing

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
		void processSamples(uint32_t deltaMs);
		float *getProcessedSamples() { return m_processedSamples; }
		Samples &getRawSamples() { return m_rawSamples; }
		float *getSampleFrequencies() { return m_sampleFrequencies; }
		
		void addSmoothing(float val);
		float getSmoothing() { return m_smoothing; }
		void setHannWindowEnabled(bool enabled);
		bool getHannWindowEnabled() { return m_hannWindowEnabled; }
		void setAWeightingEnabled(bool enabled);
		bool getAWeightingEnabled() { return m_aWeightingEnabled; }

		bool onBeat() { return m_onBeat; }
		int getBPM();
		unsigned int getBeatCount() { return m_beatCount; }

		std::string deviceName(int index);
		std::vector<int> inputDeviceList();
		int getDefaultInputDevice();

	private:
		float sampleToDb(fftwf_complex c);
		void initializeSampleStructs();
		void updateBPM(uint32_t deltaMs);

		bool m_initialized;
		std::string m_error;

		/* PortAudio */
		PaStream *m_instream;
		float m_smoothing;
		bool m_hannWindowEnabled;
		bool m_aWeightingEnabled;

		/* FFTW and Samples Processing */
		fftwf_plan m_plan;
		float *m_inBuf;
		fftwf_complex *m_outBuf;
		float m_processedSamples[AC_OUT_SIZE];
		float m_sampleFrequencies[AC_RAW_OUT_SIZE];
		int m_lastBackBuffer;
		fftwf_complex m_backBuffers[DEFAULT_BACK_BUFFER_COUNT][AC_OUT_SIZE];

		/* BPM Processing */

		uint32_t m_msBeatCooldown;
		bool m_onBeat;
		float m_energyHistBuffer[ENERGY_HIST_SIZE];
		uint32_t m_msBetweenBeats;
		uint32_t m_msLastBeat;
		CircularArray<float> m_energyHist;
		unsigned int m_beatCount;

		/* Packaged Samples data */
		Samples m_rawSamples;
	};
}

#endif /* AUDIO_CONTEXT_H */
