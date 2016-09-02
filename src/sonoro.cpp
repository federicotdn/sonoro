#include <iostream>

#include <sonoro.h>
#include <scene.h>
#include <time.h>
#include <cstdlib> 

// Scenes
#include <menu.h>
#include <line_scene.h>
#include <bars_scene.h>
#include <test_scene.h>
#include <superformula_scene.h>
#include <raw_scene.h>
#include <beat_scene.h>

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

	srand((unsigned int)time(nullptr));

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

	//Uint32 lastTicks = SDL_GetTicks();

	// ============ ADD SCENES HERE ============
	std::vector<Scene*> initialScenes =
	{
		new Menu(*this),
		new BarsScene(*this),
		new LineScene(*this),
		new TestScene(*this),
		new SuperformulaScene(*this),
		new RawScene(*this),
		new BeatScene(*this)
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

		// Update FPS and delta ms
		//Uint32 currentTicks = SDL_GetTicks();
		//Uint32 difference = currentTicks - lastTicks;
		//lastTicks = currentTicks;
		if (loopTicks % 10 == 0)
		{
			//fps = 1000 / (difference != 0 ? difference : 1);
		}

		//m_renderContext.setDeltaMs(difference);

		// Process audio
		//m_audioContext.processSamples(difference);

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

		glClear(GL_COLOR_BUFFER_BIT);

		if (!skipSceneDraw)
		{
			currentScene->draw();
		}

		GLFWwindow *win = m_renderContext.getWindow();

		glfwSwapBuffers(win);
		glfwPollEvents();

		loopTicks++;
		
		done |= glfwWindowShouldClose(win) == 0 ? false : true;
	}

	// Delete scenes
	for (auto it = scenes.begin(); it != scenes.end(); it++)
	{
		delete (*it);
	}

	return 0;
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
