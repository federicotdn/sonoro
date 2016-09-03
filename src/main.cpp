#include <iostream>

#include <render_context.h>
#include <audio_context.h>
#include <input_context.h>
#include <sonoro.h>

int main(int, char**)
{
	std::cout << "Starting..." << std::endl;

	so::RenderContext renderContext;
	if (renderContext.initialize(1024, 768))
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
