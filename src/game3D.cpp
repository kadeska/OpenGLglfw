#include "game3D.hpp"
#include "window/window.hpp"
#include "world/world.hpp"

//#include "window/input/inputManager.hpp"

#include "misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;
// log("-- Constructor", LogLevel::DEBUG);

Window* myWindow;
World* myWorld;

static const int WORLD_SIZE = 24;
static const float WORLD_SEED = 0;

static const float PLAYER_X = 5.0f;
static const float PLAYER_Y = 2.0f;
static const float PLAYER_Z = 5.0f;


Game3D::Game3D()
{
	// log("-- Constructor", LogLevel::DEBUG);
	log("Game3D Constructor", LogLevel::DEBUG);

	// since window has no constructor, we don't need to instantiate it
    myWindow = new Window();
	
	myWindow->initialize(PLAYER_X, PLAYER_Y, PLAYER_Z);
	myWindow->createWindow();
	//myWindow->loadOpenGL();
	
	//myWindow->initSceneRenderer();
	
	
	//myWindow->createShaderProgram();
	//myWindow->loadTextures();

	// Create world
	//myWorld = new World(myWindow->getSceneShader(), WORLD_SEED, WORLD_SIZE);
	//myWorld->createWorld(WORLD_SEED);
	//myWorld->spawnPlayer({PLAYER_X, PLAYER_Y, PLAYER_Z});
	//myWorld->test();
}

Game3D::~Game3D()
{
}

void Game3D::start()
{
	myWindow->mainLoop(myWorld);
}

void Game3D::stop()
{
	myWindow->terminateWindow();
}
