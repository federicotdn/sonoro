#include <iostream>

#include <render_context.h>
#include <audio_context.h>
#include <visualizer.h>

int main(int argc, char *argv[])
{
	std::cout << "Starting..." << std::endl;

	so::RenderContext renderContext;
	if (renderContext.initialize(640, 480))
	{
		std::cerr << "Error intializing render context: " << renderContext.getError() << std::endl;
		return EXIT_FAILURE;
	}

	so::AudioContext audioContext;
	if (audioContext.initialize())
	{
		std::cerr << "Error initializing audio context: " << audioContext.getError() << std::endl;
		return EXIT_FAILURE;
	}

	so::Visualizer visualizer(renderContext, audioContext);
	if (visualizer.initialize())
	{
		std::cerr << "Unable to initialize visualizer.  Exiting." << std::endl;
		return EXIT_FAILURE;
	}

	visualizer.run();

	return EXIT_SUCCESS;
}