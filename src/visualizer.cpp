#include <iostream>

#include <visualizer.h>
#include <menu.h>
#include <scene.h>

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
	int w, h;
	m_renderContext.getWindowSize(&w, &h);

	if (m_info.initialize(DEFAULT_FONT, DEFAULT_FONT_SIZE, w))
	{
		std::cerr << "Error initializing Visualizer info panel." << std::endl;
		return -1;
	}

	m_info.appendln("Visualizer ready.  Choose an input [1-9]:");
	m_info.appendln("");
	m_info.setPosition(0, h - DEFAULT_FONT_SIZE);

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

	Menu menu(m_renderContext, m_audioContext, m_keyboard);
	bool inMenu = true;
	if (menu.initialize())
	{
		std::cerr << "Unable to initialize menu.  Exiting run()." << std::endl;
		return;
	}

	Scene *currentScene = &menu;
	bool done = false;

	while (!done)
	{
		pollKeyboard();

		// Update
		
		bool exitScene = false;
		bool skipDraw = false;
		bool escPressed = m_keyboard[SDLK_ESCAPE];

		if (inMenu)
		{
			int menuVal = menu.update();
			if (menuVal)
			{
				currentScene = nullptr; // pick a scene
				skipDraw = true;
				inMenu = false;
				std::cout << "Exit menu." << std::endl;
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
			inMenu = (finalVal != 0 ? true : false) || escPressed;
			
			if (inMenu)
			{
				currentScene = &menu;

				std::cout << "Exit scene." << std::endl;
			}
		}

		Uint32 currentTicks = SDL_GetTicks();
		int fps = 1000 / (currentTicks - lastTicks);
		lastTicks = currentTicks;
		m_info.setText(std::to_string(fps));

		SDL_RenderClear(ren);

		if (currentScene != nullptr && !skipDraw)
		{
			currentScene->draw();
		}

		SDL_Texture *infoTex = m_info.getTexture(m_renderContext);
		SDL_RenderCopy(ren, infoTex, NULL, m_info.getRect());

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
