#include <iostream>

#include <sonoro.h>
#include <scene.h>

// Scenes
#include <menu.h>
#include <line_scene.h>
#include <bars_scene.h>
#include <test_scene.h>
#include <superformula_scene.h>

#define SMOOTHING_STEP 0.01f

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
	int h = m_renderContext.getWindowHeight();

	if (m_info.initialize(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_SIZE * 25))
	{
		std::cerr << "Error initializing Visualizer info panel." << std::endl;
		return -1;
	}

	if (m_help.initialize(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_SIZE * 35))
	{
		std::cerr << "Error initializing Visualizer help panel." << std::endl;
		return -1;
	}

	m_info.appendln("Visualizer ready.  Choose an input [1-9]:");
	m_info.appendln("");
	m_info.setPosition(0, h);
	m_info.setBackgroundColor({ 0, 100, 10 });

	setupHelpPanel();

	m_initialized = true;
	return 0;
}

void Sonoro::setupHelpPanel()
{
	m_help.appendln("======= SONORO HELP =======");
	m_help.appendln(" - Press SPACE to switch to the next scene.");
	m_help.appendln(" - Press C to switch to the previous scene.");
	m_help.appendln(" - F1 toggles the info HUD.");
	m_help.appendln(" - Use 'o' and 'p' to increase/decrease smoothing factor.");
	m_help.appendln(" - 'a' toggles A Weighting.");
	m_help.appendln(" - 'w' toggles Hann Window function usage.");
	m_help.appendln(" - 'h' toggles the Help dialogue.");
	m_help.appendln(" - 'f' toggles fullscreen mode.");
	m_help.appendln(" - Use the Menu scene to select the audio input device.");
	m_help.appendln(" - Press ESC to quit.");
	m_help.setBackgroundColor({ 115, 0, 105 });
}

int Sonoro::run()
{
	if (!m_initialized)
	{
		std::cerr << "Sonoro has not been initialized correctly.  Exiting run()." << std::endl;
		return -1;
	}

	SDL_Renderer *ren = m_renderContext.getRenderer();
	Uint32 lastTicks = SDL_GetTicks();

	// ============ ADD SCENES HERE ============
	std::vector<Scene*> initialScenes =
	{
		new Menu(*this),
		new BarsScene(*this),
		new LineScene(*this),
		new TestScene(*this),
		new SuperformulaScene(*this)
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
			std::cout << "Scene: " + initialScenes[i]->sceneName() + " loaded." << std::endl;
			scenes.push_back(initialScenes[i]);
		}
	}

	if (scenes.size() == 0)
	{
		std::cerr << "No scenes were loaded.  Exiting." << std::endl;
		return -1;
	}

	bool done = false;
	bool showHud = true, showHelp = false;
	int currentSceneIndex = 0, fps = 0, loopTicks = 0;

	// Main loop
	while (!done)
	{
		// Input
		m_inputContext.pollInputs();

		checkGlobalActions();

		// Process audio
		m_audioContext.processSamples();

		// Update FPS and delta ms
		Uint32 currentTicks = SDL_GetTicks();
		Uint32 difference = currentTicks - lastTicks;
		lastTicks = currentTicks;
		if (loopTicks % 10 == 0)
		{
			fps = 1000 / (difference != 0 ? difference : 1);
		}
		m_renderContext.setDeltaMs(difference);

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

		if (m_inputContext.actionActivated(SonoroAction::HELP))
		{
			showHelp = !showHelp;
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
				currentSceneIndex = (int)scenes.size() - 1;
			}
		}
		else
		{
			currentScene->update();
		}

		// Draw

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);

		if (!skipSceneDraw)
		{
			currentScene->draw();
		}

		if (showHud)
		{
			updateHud(fps, currentScene->sceneName());
			SDL_Texture *infoTex = m_info.getTexture(m_renderContext);
			SDL_RenderCopy(ren, infoTex, NULL, m_info.getRect());
		}

		if (showHelp)
		{
			SDL_Texture *helpTex = m_help.getTexture(m_renderContext);
			SDL_Rect *helpRect = m_help.getRect();
			int x = (m_renderContext.getWindowWidth() - helpRect->w) / 2;
			int y = (m_renderContext.getWindowHeight() - helpRect->h) / 2;
			m_help.setPosition(x, y);
			SDL_RenderCopy(ren, helpTex, NULL, helpRect);
		}

		SDL_RenderPresent(ren);
		loopTicks++;
	}

	// Delete scenes
	for (auto it = scenes.begin(); it != scenes.end(); it++)
	{
		delete (*it);
	}

	return 0;
}

void Sonoro::updateHud(int fps, std::string sceneName)
{
	m_info.clear();
	m_info.appendln("FPS: " + std::to_string(fps));
	m_info.appendln("Smoothing: " + std::to_string(m_audioContext.getSmoothing()));
	m_info.append("Hann Window: " + std::to_string(m_audioContext.getHannWindowEnabled()));
	m_info.appendln(", A Weighting: " + std::to_string(m_audioContext.getAWeightingEnabled()));
	m_info.appendln("Current scene: " + sceneName + " (press h for Help)");
	m_info.setPosition(0, m_renderContext.getWindowHeight() - m_info.getRect()->h);
}

void Sonoro::checkGlobalActions()
{
	if (m_inputContext.actionActivated(SonoroAction::SMOOTH_DOWN))
	{
		m_audioContext.addSmoothing(-SMOOTHING_STEP);
	}
	else if (m_inputContext.actionActivated(SonoroAction::SMOOTH_UP))
	{
		m_audioContext.addSmoothing(SMOOTHING_STEP);
	}

	if (m_inputContext.actionActivated(SonoroAction::TOGGLE_AWEIGHTING))
	{
		m_audioContext.setAWeightingEnabled(!m_audioContext.getAWeightingEnabled());
	}
	
	if (m_inputContext.actionActivated(SonoroAction::TOGGLE_HANNWINDOW))
	{
		m_audioContext.setHannWindowEnabled(!m_audioContext.getHannWindowEnabled());
	}

	if (m_inputContext.actionActivated(SonoroAction::TOGGLE_FULLSCREEN))
	{
		m_renderContext.setFullscreen(!m_renderContext.getFullscreen());
	}
}
