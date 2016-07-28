#ifndef AUDIO_CONTEXT_H
#define AUDIO_CONTEXT_H

#include <string>
#include <vector>

#include <portaudio.h>
#include <fftw3.h>

namespace so
{
	class AudioContext
	{
	public:
		AudioContext();
		~AudioContext();

		int initialize();
		std::string getError() { return m_error; }
		int setInputDevice(int index, int channels, int sampleRate, int framesPerBuffer);
		std::string deviceName(int index);
		std::vector<int> inputDeviceList();
		int getDefaultInputDevice();

	private:
		bool m_initialized;
		std::string m_error;

		/* PortAudio */
		PaStream *m_instream;
		int m_channels;
		int m_sampleRate;
		int m_framesPerBuffer;

		/* FFTW */
		fftw_plan *m_plan;
		float *m_inBuf;
		fftwf_complex *m_outBuf;
	};
}

#endif /* AUDIO_CONTEXT_H */
