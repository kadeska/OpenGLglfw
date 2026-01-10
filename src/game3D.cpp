
#include "window/window.hpp"
#include "world/world.hpp"
#include "render/loadingScreenRenderer.hpp"
#include "misc/stateManager.hpp"
#include "world/worldGenerator.hpp"
#include "misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

#include "game3D.hpp"

static const int WORLD_SIZE = 24;
static const float WORLD_SEED = 0;

static const float PLAYER_X = 5.0f;
static const float PLAYER_Y = 2.0f;
static const float PLAYER_Z = 5.0f;



Window* myWindow;
World* world;

StateManager::GameStateManager Game3D::gameStateManager;
LoadingScreenRenderer* loadingScreenRenderer = nullptr;
SceneRenderer* sceneRenderer = nullptr;
WorldGenerator* worldGenerator = nullptr;


static void onEnterPlaying() 
{
	if (world) 
	{
		sceneRenderer->setWorld(world);
	}
}

static void onExitPlaying() {}

static void onEnterLoading()
{
	// do loading tasks here
	log("LOADING", LogLevel::STATE);
	if (!sceneRenderer->hasBeenInitialized()) 
	{
		sceneRenderer->initSceneRenderer();
	}
	
	if (!worldGenerator) 
	{
		worldGenerator = new WorldGenerator(sceneRenderer->getRenderablesRef());
		worldGenerator->generateDefaultWorld(world);
	}

	log("DONE LOADING", LogLevel::STATE);
	myWindow->getGameState().setState(StateManager::GameState::PLAYING);
	
}

static void onExitLoading()
{
	log("Exit LOADING", LogLevel::DEBUG);
}

Game3D::Game3D()
{
	log("Game3D Constructor", LogLevel::DEBUG);
	myWindow = new Window(gameStateManager);
	loadingScreenRenderer = new LoadingScreenRenderer(myWindow);
	sceneRenderer = new SceneRenderer(myWindow->getWindowSize().x, myWindow->getWindowSize().y);
	
	// passing the pointers to the window. pointers must be set first.
	myWindow->setLoadingRenderer(loadingScreenRenderer);
	myWindow->setSceneRenderer(sceneRenderer);

	myWindow->initialize(5.0f, 2.0f, 5.0f);
	myWindow->createWindow();
	loadingScreenRenderer->init();
	

	gameStateManager.onEnter(StateManager::GameState::LOADING, onEnterLoading);
	gameStateManager.onExit(StateManager::GameState::LOADING, onExitLoading);

	gameStateManager.onEnter(StateManager::GameState::PLAYING, onEnterPlaying);
	gameStateManager.onExit(StateManager::GameState::PLAYING, onExitPlaying);

	gameStateManager.setState(StateManager::GameState::MAIN_MENU);
	// start loading game data and once finished, switch to PLAYING state
}

Game3D::~Game3D()
{
	log("Game3D Destructor", LogLevel::DEBUG);
	delete myWindow;
	//delete world;
}

LoadingScreenRenderer* Game3D::getLoadingScreenRenderer()
{
	return loadingScreenRenderer;
}

SceneRenderer* Game3D::getSceneRenderer()
{
	return sceneRenderer;
}

void Game3D::start()
{
	myWindow->mainLoop();
}

void Game3D::stop()
{
	myWindow->terminateWindow();
}


