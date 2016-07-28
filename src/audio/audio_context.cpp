#include <audio_context.h>

using namespace so;

#define DEFAULT_CHAN_COUNT 1
#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_FRAMES_PER_BUFFER 64

AudioContext::AudioContext() :
	m_initialized(false),
	m_plan(nullptr),
	m_instream(nullptr),
	m_channels(DEFAULT_CHAN_COUNT),
	m_sampleRate(DEFAULT_SAMPLE_RATE),
	m_framesPerBuffer(DEFAULT_FRAMES_PER_BUFFER),
	m_inBuf(nullptr),
	m_outBuf(nullptr)
{
}

AudioContext::~AudioContext()
{
	if (!m_initialized)
	{
		return;
	}

	Pa_Terminate();
}

int AudioContext::initialize()
{
	if (m_initialized)
	{
		m_error = "AudioContext is already initialized.";
		return -1;
	}

	PaError err = Pa_Initialize();
	if (err != paNoError) {
		m_error = "Unable to initialize PortAudio: " + std::string(Pa_GetErrorText(err));
		return -1;
	}

	bool foundInput = false;
	int deviceCount = Pa_GetDeviceCount();
	const PaDeviceInfo *info = nullptr;
	for (int i = 0; i < deviceCount && !foundInput; i++)
	{
		info = Pa_GetDeviceInfo(i);
		if (info->maxInputChannels > 0)
		{
			foundInput = true;
		}
	}

	if (!foundInput)
	{
		Pa_Terminate();
		m_error = "No audio input device found.";
		return -1;
	}

	m_initialized = true;
	return 0;
}

int AudioContext::setInputDevice(int index, int channels, int sampleRate, int framesPerBuffer)
{
	int deviceCount = Pa_GetDeviceCount();
	if (deviceCount <= 0 || index < 0 || index > deviceCount - 1)
	{
		return -1;
	}

	PaStreamParameters inputParameters = { 0 };

	inputParameters.device = index;
	inputParameters.channelCount = channels;
	inputParameters.sampleFormat = paFloat32; /* must match FFTW precision */
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = nullptr;

	PaStream *stream = nullptr;

	PaError err = Pa_OpenStream(&stream, &inputParameters, nullptr, sampleRate, framesPerBuffer, paClipOff, nullptr, nullptr);
	if (err != paNoError) {
		m_error = "Unable to open instream: " + std::string(Pa_GetErrorText(err));
		return -1;
	}

	return 0;
}

std::string AudioContext::deviceName(int index)
{
	if (index > Pa_GetDeviceCount() - 1 || index < 0)
	{
		return "Unsupported device";
	}

	const PaDeviceInfo *info = Pa_GetDeviceInfo(index);
	return std::string(info->name);
}

std::vector<int> AudioContext::inputDeviceList()
{
	std::vector<int> devices;

	int deviceCount = Pa_GetDeviceCount();
	if (deviceCount <= 0)
	{
		return devices;
	}

	const PaDeviceInfo *info = nullptr;
	for (int i = 0; i < deviceCount; i++)
	{
		info = Pa_GetDeviceInfo(i);
		if (info->maxInputChannels > 0)
		{
			devices.push_back(i);
		}
	}

	return devices;
}

int AudioContext::getDefaultInputDevice()
{
	return Pa_GetDefaultInputDevice();
}
