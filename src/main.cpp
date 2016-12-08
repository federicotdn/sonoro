#include <iostream>

#include <render_context.h>
#include <audio_context.h>
#include <input_context.h>
#include <sonoro.h>

int main(int, char**)
{
	std::cout << "Sonoro build 1" << std::endl;
	std::cout << "Starting..." << std::endl;

	int w = 0, h = 0;
	char c = 0;

	std::cout << "Enter resolution Width:" << std::endl;
	std::cin >> w;
	std::cout << "Enter resolution Height:" << std::endl;
	std::cin >> h;
	std::cout << "Use fullscreen? [y/n]: ";
	std::cin >> c;

	if (w == 0 || h == 0) {
		std::cerr << "Invalid resolution" << std::endl;
		return EXIT_FAILURE;
	}

	so::RenderContext renderContext;
	if (renderContext.initialize(w, h, c == 'y'))
	{
		std::cerr << "Error intializing render context: " << renderContext.getError() << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "Initialized Render Context. Info:" << std::endl;
		std::cout << renderContext.getGLInfoString();
	}

	so::AudioContext audioContext;
	if (audioContext.initialize())
	{
		std::cerr << "Error initializing audio context: " << audioContext.getError() << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "Initialized Audio Context." << std::endl;
	}

	so::InputContext inputContext(renderContext);

	so::Sonoro app(renderContext, audioContext, inputContext);
	if (app.initialize())
	{
		std::cerr << "Unable to initialize visualizer.  Exiting." << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "Initialized Sonoro." << std::endl;
	}

	int ret = app.run();
	return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
