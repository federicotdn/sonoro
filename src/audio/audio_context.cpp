#include <audio_context.h>
#include <a_weighting.h>

#include <math.h>
#include <limits>

#define MAX_BPM 130.0f
#define MIN_MS_BETWEEN_BEATS (60000.0f / MAX_BPM)

using namespace so;

AudioContext::AudioContext() :
	m_initialized(false),
	m_plan(nullptr),
	m_instream(nullptr),
	m_inBuf(nullptr),
	m_outBuf(nullptr),
	m_processedSamples{0},
	m_sampleFrequencies{0},
	m_backBuffers{0},
	m_lastBackBuffer(0),
	m_smoothing(DEFAULT_SMOOTHING),
	m_hannWindowEnabled(true),
	m_aWeightingEnabled(true),
	m_rawSamples(),
	m_onBeat(false),
	m_msBeatCooldown((uint32_t)MIN_MS_BETWEEN_BEATS + 1),
	m_energyHistBuffer{0},
	m_energyHist(m_energyHistBuffer, ENERGY_HIST_SIZE),
	m_msBetweenBeats((uint32_t)MIN_MS_BETWEEN_BEATS),
	m_msLastBeat((uint32_t)MIN_MS_BETWEEN_BEATS),
	m_beatCount(0)
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

void AudioContext::initializeSampleStructs()
{
	m_rawSamples.values = m_inBuf;
	m_rawSamples.min = m_rawSamples.mean = 0;
	m_rawSamples.max = 1;
	m_rawSamples.size = AC_IN_SIZE;

	m_frequencyAmplitudes.values = m_processedSamples;
	m_frequencyAmplitudes.min = m_frequencyAmplitudes.mean = 0;
	m_frequencyAmplitudes.max = 1;
	m_frequencyAmplitudes.size = AC_OUT_SIZE;
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
	m_outBuf = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * AC_RAW_OUT_SIZE);

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

	initializeSampleStructs();
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

	inputParameters.device = index;
	inputParameters.channelCount = DEFAULT_CHAN_COUNT;
	inputParameters.sampleFormat = paFloat32; /* must match FFTW precision */
	inputParameters.suggestedLatency = 0;
	inputParameters.hostApiSpecificStreamInfo = nullptr;

	float sampleRate = DEFAULT_SAMPLE_RATE;
	for (int i = 0; i < AC_RAW_OUT_SIZE; i++)
	{
		m_sampleFrequencies[i] = i * sampleRate / DEFAULT_FRAMES_PER_BUFFER;
	}

	PaError err = Pa_OpenStream(&m_instream, &inputParameters, nullptr, sampleRate, DEFAULT_FRAMES_PER_BUFFER, paNoFlag, nullptr, nullptr);
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

void AudioContext::processSamples(uint32_t deltaMs)
{
	if (!m_initialized || m_instream == nullptr || Pa_IsStreamActive(m_instream) == 0)
	{
		for (int i = 0; i < DEFAULT_FRAMES_PER_BUFFER; i++) {
			m_inBuf[i] = 0;
		}

		for (int i = 0; i < AC_OUT_SIZE; i++)
		{
			m_processedSamples[i] = 0; 
		}

		initializeSampleStructs();

		return;
	}

	// Returns PaInputOverflowed if data was discarded (ignore)
	Pa_ReadStream(m_instream, m_inBuf, DEFAULT_FRAMES_PER_BUFFER);

	float maxSample = std::numeric_limits<float>::lowest();
	float minSample = std::numeric_limits<float>::max();
	float mean = 0;

	for (int i = 0; i < DEFAULT_FRAMES_PER_BUFFER; i++)
	{
		// Hann window function
		if (m_hannWindowEnabled)
		{
			float temp = sinf(PI_FLOAT * i / (DEFAULT_FRAMES_PER_BUFFER - 1));
			m_inBuf[i] *= temp * temp;
		}

		float sample = m_inBuf[i];
		
		if (sample > maxSample)
		{
			maxSample = sample;
		}

		if (sample < minSample)
		{
			minSample = sample;
		}

		mean += sample;
	}

	mean /= DEFAULT_FRAMES_PER_BUFFER;
	
	m_rawSamples.max = maxSample;
	m_rawSamples.min = minSample;
	m_rawSamples.mean = mean;

	updateBPM(deltaMs);

	fftwf_execute(m_plan);

	maxSample = std::numeric_limits<float>::lowest();
	minSample = std::numeric_limits<float>::max();
	mean = 0;

	int backBufferIndex = m_lastBackBuffer;
	m_lastBackBuffer++;
	if (m_lastBackBuffer == DEFAULT_BACK_BUFFER_COUNT)
	{
		m_lastBackBuffer = 0;
	}

	int j = 0;
	for (int i = AC_RAW_OUT_OFFSET; i < AC_OUT_SIZE; i++)
	{
		int outputIndex = i - AC_RAW_OUT_OFFSET;
		float real = m_outBuf[i][0];
		float imag = m_outBuf[i][1];

		m_backBuffers[backBufferIndex][i][0] = real;
		m_backBuffers[backBufferIndex][i][1] = imag;
		
		float realBuffered = 0, imagBuffered = 0;

		int currentBackBuffer = backBufferIndex + 1;
		for (int k = 0; k < DEFAULT_BACK_BUFFER_COUNT - 1; k++)
		{
			if (currentBackBuffer == DEFAULT_BACK_BUFFER_COUNT)
			{
				currentBackBuffer = 0;
			}

			realBuffered += m_backBuffers[currentBackBuffer][i][0];
			imagBuffered += m_backBuffers[currentBackBuffer][i][1];
			currentBackBuffer++;
		}

		realBuffered /= (DEFAULT_BACK_BUFFER_COUNT - 1);
		imagBuffered /= (DEFAULT_BACK_BUFFER_COUNT - 1);
		fftwf_complex averagedSample;
		averagedSample[0] = real * (1 - m_smoothing) + realBuffered * m_smoothing;
		averagedSample[1] = imag * (1 - m_smoothing) + imagBuffered * m_smoothing;

		m_processedSamples[outputIndex] = sampleToDb(averagedSample);

		if (m_aWeightingEnabled)
		{
			while (m_sampleFrequencies[i] > a_weights[j].freq && j < sizeof(a_weights))
			{
				j++;
			}

			float aWeightDb = 0;
			if (j == 0 || j >= sizeof(a_weights))
			{
				if (j > sizeof(a_weights))
				{
					j = sizeof(a_weights) - 1;
				}
				aWeightDb = a_weights[i].db;
			}
			else
			{
				float db0 = a_weights[j - 1].db;
				float freq0 = a_weights[j - 1].freq;
				float db1 = a_weights[j].db;
				float freq1 = a_weights[j].freq;
				aWeightDb = db0 + (db1 - db0) * ((m_sampleFrequencies[i] - freq0) / (freq1 - freq0));
			}

			m_processedSamples[outputIndex] += aWeightDb;
		}

		if (m_processedSamples[outputIndex] > maxSample)
		{
			maxSample = m_processedSamples[outputIndex];
		}

		if (m_processedSamples[outputIndex] < minSample)
		{
			minSample = m_processedSamples[outputIndex];
		}

		mean += m_processedSamples[outputIndex];
	}

	mean /= AC_OUT_SIZE;

	m_frequencyAmplitudes.max = maxSample;
	m_frequencyAmplitudes.min = minSample;
	m_frequencyAmplitudes.mean = mean;
}

float AudioContext::sampleToDb(fftwf_complex c)
{
	float real = c[0];
	float imag = c[1];
	float absSquared = (real * real) + (imag * imag);
	if (absSquared == 0.0f) {
		return 0;
	}
	return 10.0f * log10f((real * real) + (imag * imag));
}

void AudioContext::addSmoothing(float val)
{
	m_smoothing += val;
	if (m_smoothing < 0)
	{
		m_smoothing = 0;
	}
	else if (m_smoothing > 1)
	{
		m_smoothing = 1;
	}
}

void AudioContext::updateBPM(uint32_t deltaMs)
{
	m_onBeat = false;
	m_msLastBeat += deltaMs;

	float meanEnergy = 0;
	for (int i = 0; i < m_energyHist.getCount(); i++)
	{
		meanEnergy += m_energyHist.m_buffer[i];
	}
	meanEnergy /= m_energyHist.getCount();

	float variance = 0;
	for (int i = 0; i < m_energyHist.getCount(); i++)
	{
		float tmp = m_energyHist.m_buffer[i] - meanEnergy;
		variance += tmp * tmp;
	}
	variance /= m_energyHist.getCount();

	float instantEnergy = 0;
	for (int i = 0; i < DEFAULT_FRAMES_PER_BUFFER; i++)
	{
		instantEnergy += (m_inBuf[i]) * (m_inBuf[i]);
	}
	instantEnergy /= DEFAULT_FRAMES_PER_BUFFER;

	m_energyHist.addValue(instantEnergy);

	float C = (-0.0025714f * variance) + 1.51142857f;

	bool beat = (instantEnergy > C * meanEnergy);

	m_msBeatCooldown += deltaMs;
	if (m_msBeatCooldown > MIN_MS_BETWEEN_BEATS && beat)
	{
		m_onBeat = true;
		m_beatCount++;
		m_msBeatCooldown = 0;
		
		m_msBetweenBeats = (m_msBetweenBeats + m_msLastBeat) / 2;
		m_msLastBeat = 0;
	}
}

void AudioContext::setHannWindowEnabled(bool enabled)
{
	m_hannWindowEnabled = enabled;
}

void AudioContext::setAWeightingEnabled(bool enabled)
{
	m_aWeightingEnabled = enabled;
}

int AudioContext::getBPM()
{
	double separation = 60.0 * 1000.0 / (double)m_msBetweenBeats;
	return (int)separation;
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
