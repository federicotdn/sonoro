#include <audio_context.h>

using namespace so;

AudioContext::AudioContext() :
	m_initialized(false),
	m_plan(nullptr),
	m_instream(nullptr),
	m_inBuf(nullptr),
	m_outBuf(nullptr),
	m_processedSamples{0},
	m_sampleFrequencies{0}
{
}

AudioContext::~AudioContext()
{
	if (!m_initialized)
	{
		return;
	}

	if (m_plan != nullptr)
	{
		fftwf_destroy_plan(m_plan);
	}

	fftwf_free(m_inBuf);
	fftwf_free(m_outBuf);

	if (m_instream != nullptr)
	{
		Pa_CloseStream(m_instream);
		m_instream = nullptr;
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

	m_inBuf = (float*)fftwf_malloc(sizeof(float) * DEFAULT_FRAMES_PER_BUFFER);
	m_outBuf = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * DEFAULT_OUT_SIZE);

	if (m_inBuf == nullptr || m_outBuf == nullptr)
	{
		fftwf_free(m_inBuf);
		fftwf_free(m_outBuf);
		m_outBuf = nullptr;
		m_inBuf = nullptr;

		m_error = "FFTW Malloc returned NULL (out of memory).";
		Pa_Terminate();
		return -1;
	}

	m_plan = fftwf_plan_dft_r2c_1d(DEFAULT_FRAMES_PER_BUFFER, m_inBuf, m_outBuf, FFTW_MEASURE);
	if (m_plan == nullptr)
	{
		fftwf_free(m_inBuf);
		fftwf_free(m_outBuf);
		m_outBuf = nullptr;
		m_inBuf = nullptr;

		m_error = "Unable to create FFTW plan.";
		Pa_Terminate();
		return -1;
	}

	m_initialized = true;
	return 0;
}

int AudioContext::setInputDevice(int index)
{
	int deviceCount = Pa_GetDeviceCount();
	if (deviceCount <= 0 || index < 0 || index > deviceCount - 1)
	{
		m_error = "PortAudio: invalid device index.";
		return -1;
	}

	if (m_instream != nullptr)
	{
		stopStream();
		Pa_CloseStream(m_instream);
		m_instream = nullptr;
	}

	PaStreamParameters inputParameters = { 0 };
	const PaDeviceInfo *info = Pa_GetDeviceInfo(inputParameters.device);

	inputParameters.device = index;
	inputParameters.channelCount = DEFAULT_CHAN_COUNT;
	inputParameters.sampleFormat = paFloat32; /* must match FFTW precision */
	inputParameters.suggestedLatency = 0;
	inputParameters.hostApiSpecificStreamInfo = nullptr;

	double sampleRate = info->defaultSampleRate;
	for (int i = 0; i < DEFAULT_OUT_SIZE; i++)
	{
		m_sampleFrequencies[i] = i * sampleRate / DEFAULT_FRAMES_PER_BUFFER;
	}

	PaError err = Pa_OpenStream(&m_instream, &inputParameters, nullptr, sampleRate, DEFAULT_FRAMES_PER_BUFFER, paClipOff, nullptr, nullptr);
	if (err != paNoError) {
		m_error = "Unable to open instream: " + std::string(Pa_GetErrorText(err));
		m_instream = nullptr;
		return -1;
	}

	return startStream();
}

int AudioContext::startStream()
{
	if (!m_initialized || m_instream == nullptr)
	{
		m_error = "AudioContext is not initialized.";
		return -1;
	}

	if (Pa_IsStreamActive(m_instream) == 1)
	{
		return 0;
	}

	PaError err = Pa_StartStream(m_instream);
	if (err != paNoError) {
		m_error = "Unable to start instream: " + std::string(Pa_GetErrorText(err));
		return -1;
	}

	return 0;
}

int AudioContext::stopStream()
{
	if (!m_initialized || m_instream == nullptr)
	{
		m_error = "AudioContext is not initialized.";
		return -1;
	}

	if (Pa_IsStreamActive(m_instream) == 0)
	{
		return 0;
	}

	PaError err = Pa_StopStream(m_instream);
	if (err != paNoError) {
		m_error = "Unable to stop instream: " + std::string(Pa_GetErrorText(err));
		return -1;
	}

	return 0;
}

void AudioContext::processSamples()
{
	if (!m_initialized || m_instream == nullptr || Pa_IsStreamActive(m_instream) == 0)
	{
		for (int i = 0; i < DEFAULT_OUT_SIZE; i++)
		{
			m_processedSamples[i] = 0;
		}

		return;
	}

	// Returns PaInputOverflowed if data was discarded (ignore)
	Pa_ReadStream(m_instream, m_inBuf, DEFAULT_FRAMES_PER_BUFFER);

	for (int i = 0; i < DEFAULT_OUT_SIZE; i++) {
		float temp = sinf(PI_FLOAT * i / (DEFAULT_OUT_SIZE - 1));
		m_inBuf[i] *= temp * temp;
	}

	fftwf_execute(m_plan);

	float maxSample = std::numeric_limits<float>::lowest();
	float minSample = std::numeric_limits<float>::max();

	for (int i = 0; i < DEFAULT_OUT_SIZE; i++) {
		float real = m_outBuf[i][0];
		float imag = m_outBuf[i][1];

		float absolute = sqrtf((real * real) + (imag * imag));
		//float absolute = 10.0f * log10f((real * real) + (imag * imag));
		//float absolute = (m_inBuf[i * 2] + m_inBuf[i * 2 - 1])/2;
		//float absolute = powf(2, (real * real) + (imag * imag));

		m_processedSamples[i] = absolute;

		if (absolute > maxSample)
		{
			maxSample = absolute;
		}

		if (absolute < minSample)
		{
			minSample = absolute;
		}
	}

	for (int i = 0; i < DEFAULT_OUT_SIZE; i++) {
		m_processedSamples[i] -= minSample;
		m_processedSamples[i] /= (maxSample - minSample);
	}
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
