#include <portaudio.h>
#include <fftw3.h>

#include <iostream>
#include <cmath>

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
	//float values[FRAMES_PER_BUFFER];
	float min = 1, max = -1;

	float *in;
	fftwf_complex *out;
	fftwf_plan plan;

	in = (float*)fftwf_malloc(sizeof(float) * FRAMES_PER_BUFFER);
	int out_size = (FRAMES_PER_BUFFER / 2) + 1;
	out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * out_size);

	float absolute_out[(FRAMES_PER_BUFFER / 2) + 1];

	std::cout << "Creating FFTW plan... ";
	plan = fftwf_plan_dft_r2c_1d(FRAMES_PER_BUFFER, in, out, FFTW_MEASURE);
	std::cout << "Done." << std::endl;

	int skipper = 0;

	for (int i = 0; i < (seconds * SAMPLE_RATE) / FRAMES_PER_BUFFER; i++) {
		err = Pa_ReadStream(stream, in, FRAMES_PER_BUFFER);
		if (err != paNoError) {
			std::cerr << "Error reading stream: " << Pa_GetErrorText(err) << std::endl;
			return EXIT_FAILURE;
		}

		float avg = 0;
		for (int j = 0; j < FRAMES_PER_BUFFER; j++) {
			avg += in[j];
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
			//std::cout << "val: " << avg << std::endl;
		}

		//FFTW
		fftwf_execute(plan);
		float low = 0, high = 0;
		int low_count = 0, high_count = 0;

		for (int i = 0; i < out_size; i++) {
			float real = out[i][0];
			float imag = out[i][1];
			float absolute = 10.0 * log10f((real * real) + (imag * imag));//sqrtf((real * real) + (imag * imag));
			absolute_out[i] = absolute;

			if (i < out_size / 2) {
				low_count++;
				low = low + ((absolute - low) / low_count);
			}
			else {
				high_count++;
				high = high + ((absolute - high) / high_count);
			}
		}

		if (skipper % 50 == 0) {
			//std::cout << "low: " << low << std::endl;
			//std::cout << "high: " << high << std::endl;

			if (low < 0.01) {
				low = 0;
			}

			if (high < 0.01) {
				high = 0;
			}

			for (int k = 0; k < low * 60; k++) {
				std::cout << "X";
			}
			std::cout << std::endl;

			for (int k = 0; k < high * 60; k++) {
				std::cout << "G";
			}
			std::cout << std::endl;
		}
		skipper++;
			 
		//std::cout << "absolutes 0: " << absolute_out[0] << std::endl;
	}

	std::cout << "new min: " << min << std::endl;
	std::cout << "new max: " << max << std::endl;

	fftwf_destroy_plan(plan);
	fftwf_free(in);
	fftwf_free(out);

	Pa_Terminate();
}