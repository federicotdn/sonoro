#include <iostream>

#include <visualizer.h>
#include <menu.h>
#include <scene.h>

#include <line_scene.h>
#include <bars_scene.h>

#include <SDL.h>

using namespace so;

Visualizer::Visualizer(RenderContext &renderContext, AudioContext &audioContext) :
	m_renderContext(renderContext),
	m_audioContext(audioContext)
{
}

Visualizer::~Visualizer()
{
}

int Visualizer::initialize()
{
	int w = m_renderContext.getWindowWidth();
	int h = m_renderContext.getWindowHeight();

	if (m_info.initialize(DEFAULT_FONT, DEFAULT_FONT_SIZE, w))
	{
		std::cerr << "Error initializing Visualizer info panel." << std::endl;
		return -1;
	}

	m_info.appendln("Visualizer ready.  Choose an input [1-9]:");
	m_info.appendln("");
	m_info.setPosition(0, h - DEFAULT_FONT_SIZE * 2);

	return 0;
}

void Visualizer::run()
{
	if (!m_initialized)
	{
		std::cerr << "Visualizer has not been initialized.  Exiting run()." << std::endl;
		return;
	}

	SDL_Renderer *ren = m_renderContext.getRenderer();
	Uint32 lastTicks = SDL_GetTicks() - 1;

	Menu::MenuResult menuResult;
	Menu menu(m_renderContext, m_audioContext, m_keyboard, menuResult);
	if (menu.initialize())
	{
		std::cerr << "Unable to initialize menu.  Exiting run()." << std::endl;
		return;
	}

	LineScene lineScene(m_renderContext, m_audioContext, m_keyboard);
	BarsScene barsScene(m_renderContext, m_audioContext, m_keyboard);

	Scene *currentScene = &menu;
	bool done = false;
	bool showHud = true;

	// Main loop
	while (!done)
	{
		// Input

		pollKeyboard();
		if (m_keyboard[SDLK_o])
		{
			m_audioContext.addSmoothing(-0.05f);
		}
		else if (m_keyboard[SDLK_p]) {
			m_audioContext.addSmoothing(0.05f);
		}

		// Process audio
		m_audioContext.processSamples();

		// Update
		
		bool exitScene = false;
		bool skipDraw = false;
		bool escPressed = m_keyboard[SDLK_ESCAPE];
		bool inMenu = (currentScene == &menu);

		if (m_keyboard[SDLK_F1])
		{
			showHud = !showHud;
		}

		if (inMenu)
		{
			int menuVal = menu.update();
			if (menuVal)
			{
				currentScene = &barsScene; // pick a scene
				skipDraw = true;

				std::cout << "Enter scene." << std::endl;

				int err = m_audioContext.setInputDevice(menuResult.inputDeviceIndex);
				if (err)
				{
					std::cerr << "Error using device: " << m_audioContext.getError() << std::endl;
					currentScene = &menu;
					skipDraw = false;
				}
				else
				{
					std::cout << "Using PortAudio device index: " << menuResult.inputDeviceIndex << std::endl;
				}
			}
			done = escPressed;
		}
		else
		{
			int finalVal = 0;
			if (currentScene != nullptr)
			{
				finalVal = currentScene->update();
			} 
			bool exitToMenu = (finalVal != 0 ? true : false) || escPressed;
			
			if (exitToMenu)
			{
				currentScene = &menu;
				menu.reset();

				std::cout << "Exit scene." << std::endl;
			}
		}

		Uint32 currentTicks = SDL_GetTicks();
		Uint32 difference = currentTicks - lastTicks;
		int fps = 1000 / (difference != 0 ? difference : 1);
		lastTicks = currentTicks;

		// Draw

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);

		if (currentScene != nullptr && !skipDraw)
		{
			currentScene->draw();
		}

		if (showHud)
		{
			m_info.clear();
			m_info.appendln("FPS: " + std::to_string(fps));
			m_info.appendln("Smoothing: " + std::to_string(m_audioContext.getSmoothing()));
			SDL_Texture *infoTex = m_info.getTexture(m_renderContext);
			SDL_RenderCopy(ren, infoTex, NULL, m_info.getRect());
		}

		SDL_RenderPresent(ren);
	}
}

void Visualizer::pollKeyboard()
{
	m_keyboard.clear();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			m_keyboard[event.key.keysym.sym] = false;
			break;
		case SDL_KEYUP:
			m_keyboard[event.key.keysym.sym] = true;
			break;
		}
	}
}
