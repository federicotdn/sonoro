#include <iostream>

#include <sonoro.h>
#include <menu.h>
#include <scene.h>

#include <line_scene.h>
#include <bars_scene.h>

#include <SDL.h>

using namespace so;

Sonoro::Sonoro(RenderContext &renderContext, AudioContext &audioContext, InputContext &inputContext) :
	m_renderContext(renderContext),
	m_audioContext(audioContext),
	m_inputContext(inputContext),
	m_initialized(false)
{
}

Sonoro::~Sonoro()
{
}

int Sonoro::initialize()
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

	m_initialized = true;
	return 0;
}

int Sonoro::run()
{
	if (!m_initialized)
	{
		std::cerr << "Sonoro has not been initialized correctly.  Exiting run()." << std::endl;
		return -1;
	}

	SDL_Renderer *ren = m_renderContext.getRenderer();
	Uint32 lastTicks = SDL_GetTicks() - 1;

	// ============ ADD SCENES HERE ============
	std::vector<Scene*> initialScenes =
	{
		new Menu(*this),
		new LineScene(*this),
		new BarsScene(*this)
	};
	// =========================================

	std::vector<Scene*> scenes;
	for (int i = 0; i < initialScenes.size(); i++)
	{
		int ret = initialScenes[i]->initialize();
		if (ret != 0)
		{
			std::cerr << "Scene: " + initialScenes[i]->sceneName() + " failed to initialize." << std::endl;
			std::cerr << "Error code: " << std::to_string(ret) << std::endl;
			delete initialScenes[i];
		}
		else
		{
			scenes.push_back(initialScenes[i]);
		}
	}

	if (scenes.size() == 0)
	{
		std::cerr << "No scenes were loaded.  Exiting." << std::endl;
		return -1;
	}

	bool done = false;
	bool showHud = true;
	int currentSceneIndex = 0;

	// Main loop
	while (!done)
	{
		// Input
		m_inputContext.pollInputs();

		if (m_inputContext.actionActivated(SonoroAction::SMOOTH_DOWN))
		{
			m_audioContext.addSmoothing(-0.05f);
		}
		else if (m_inputContext.actionActivated(SonoroAction::SMOOTH_UP)) {
			m_audioContext.addSmoothing(0.05f);
		}

		// Process audio
		m_audioContext.processSamples();

		// Update
		
		bool skipSceneDraw = false;
		Scene *currentScene = scenes[currentSceneIndex];

		if (m_inputContext.actionActivated(SonoroAction::EXIT))
		{
			done = true;
		}

		if (m_inputContext.actionActivated(SonoroAction::TOGGLE_HUD))
		{
			showHud = !showHud;
		}

		if (m_inputContext.actionActivated(SonoroAction::NEXT_SCENE))
		{
			skipSceneDraw = true;
			currentSceneIndex++;
			if (currentSceneIndex == scenes.size())
			{
				currentSceneIndex = 0;
			}
		}
		else if (m_inputContext.actionActivated(SonoroAction::PREV_SCENE))
		{
			skipSceneDraw = true;
			currentSceneIndex--;
			if (currentSceneIndex == -1)
			{
				currentSceneIndex = scenes.size() - 1;
			}
		}
		else
		{
			currentScene->update();
		}

		Uint32 currentTicks = SDL_GetTicks();
		Uint32 difference = currentTicks - lastTicks;
		int fps = 1000 / (difference != 0 ? difference : 1);
		lastTicks = currentTicks;

		// Draw

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);

		if (!skipSceneDraw)
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

	// Delete scenes
	for (auto it = scenes.begin(); it != scenes.end(); it++)
	{
		delete (*it);
	}

	return 0;
}
