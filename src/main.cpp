#include "portaudio.h"

#include <iostream>

#define CHAN_COUNT 1
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64

int main(int argc, char *argv[]) {
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		std::cerr << "Unable to initialize PortAudio: " << Pa_GetErrorText(err) << std::endl;
		return EXIT_FAILURE;
	}

	PaStreamParameters inputParameters = { 0 };

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	inputParameters.channelCount = CHAN_COUNT;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	PaStream *stream = nullptr;

	err = Pa_OpenDefaultStream(&stream, CHAN_COUNT, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, nullptr, nullptr);
	if (err != paNoError) {
		std::cerr << "Unable to open in stream: " << Pa_GetErrorText(err) << std::endl;
		return EXIT_FAILURE;
	}

	err = Pa_StartStream(stream);
	if (err != paNoError) {
		std::cerr << "Unable to start in stream: " << Pa_GetErrorText(err) << std::endl;
		return EXIT_FAILURE;
	}

	int seconds = 60;
	float values[FRAMES_PER_BUFFER];
	float min = 1, max = -1;

	for (int i = 0; i < (seconds * SAMPLE_RATE) / FRAMES_PER_BUFFER; i++) {
		err = Pa_ReadStream(stream, values, FRAMES_PER_BUFFER);
		if (err != paNoError) {
			std::cerr << "Error reading stream: " << Pa_GetErrorText(err) << std::endl;
			return EXIT_FAILURE;
		}

		float avg = 0;
		for (int j = 0; j < FRAMES_PER_BUFFER; j++) {
			avg += values[j];
		}

		avg /= FRAMES_PER_BUFFER;
		//std::cout << "Val: " << avg << std::endl;

		if (avg < min) {
			min = avg;
			
		}

		if (avg > max) {
			min = avg;
		}

		if (avg > 0.05) {
			std::cout << "val: " << avg << std::endl;
		}
	}

	std::cout << "new min: " << min << std::endl;
	std::cout << "new max: " << max << std::endl;

	Pa_Terminate();
}